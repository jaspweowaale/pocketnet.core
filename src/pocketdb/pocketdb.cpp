// Copyright (c) 2018 PocketNet developers
// PocketDB general wrapper
//-----------------------------------------------------
#include "pocketdb/pocketdb.h"
#include "html.h"
#include "tools/logger.h"

#if defined(HAVE_CONFIG_H)
#include <config/pocketcoin-config.h>
#endif //HAVE_CONFIG_H
//-----------------------------------------------------
std::unique_ptr<PocketDB> g_pocketdb;
std::map<uint256, std::string> POCKETNET_DATA;
//-----------------------------------------------------
PocketDB::PocketDB()
{
    // reindexer::logInstallWriter([](int level, char* buf) {
    //     LogPrintf("=== %s\n", buf);
    // });
}
PocketDB::~PocketDB()
{
    CloseNamespaces();
}

void PocketDB::CloseNamespaces() {
    db->CloseNamespace("Service");
    db->CloseNamespace("Mempool");
    db->CloseNamespace("User");
    db->CloseNamespace("Tags");
    db->CloseNamespace("Post");
    db->CloseNamespace("Scores");
    db->CloseNamespace("Complains");
    db->CloseNamespace("UserRatings");
    db->CloseNamespace("PostRatings");
    db->CloseNamespace("SubscribesView");
    db->CloseNamespace("Subscribes");
    db->CloseNamespace("BlockingView");
    db->CloseNamespace("Blocking");
    db->CloseNamespace("Reposts");
    db->CloseNamespace("UTXO");
    db->CloseNamespace("Addresses");
    db->CloseNamespace("Comments");
    db->CloseNamespace("Comment");
}

// Check for update DB
bool PocketDB::UpdateDB() {
    // Find current version of RDB
    int current_version = 0;
    Item service_itm;
    Error err = SelectOne(Query("Service").Sort("version", true), service_itm);
    if (err.ok()) current_version = service_itm["version"].As<int>();

    // Need to update?
    if (current_version < version) {
        LogPrintf("Current version RDB=%s. Need to update RDB structure to version %s. Blockchain data will be erased and uploaded again.\n", current_version, version);

        CloseNamespaces();
        db->~Reindexer();
        db = nullptr;

        remove_all(GetDataDir() / "blocks");
        remove_all(GetDataDir() / "chainstate");
        remove_all(GetDataDir() / "indexes");
        remove_all(GetDataDir() / "pocketdb");

        return ConnectDB();
    }

    return true;
}

bool PocketDB::ConnectDB() {
    db = new Reindexer();
    Error err = db->Connect("builtin://" + (GetDataDir() / "pocketdb").string());
    if (!err.ok()) {
        LogPrintf("Cannot open Reindexer DB (%s) - %s\n", (GetDataDir() / "pocketdb").string(), err.what());
        return false;
    }

    return InitDB();
}

bool findInVector(std::vector<reindexer::NamespaceDef> defs, std::string name)
{
    return std::find_if(defs.begin(), defs.end(), [&](const reindexer::NamespaceDef& nsDef) { return nsDef.name == name; }) != defs.end();
}

bool PocketDB::Init()
{
    if (!ConnectDB()) return false;
    if (!UpdateDB()) return false;
    
    LogPrintf("Loaded Reindexer DB (%s)\n", (GetDataDir() / "pocketdb").string());

    // Save current version
    Item service_new_item = db->NewItem("Service");
    service_new_item["version"] = version;
    UpsertWithCommit("Service", service_new_item);

    // Turn on/off statistic
    Item conf_item = db->NewItem("#config");
    conf_item.FromJSON(R"json({
		"type":"profiling", 
		"profiling":{
			"queriesperfstats":false,
			"perfstats":false,
			"memstats":false,
            "activitystats": false
		}
	})json");
    UpsertWithCommit("#config", conf_item);

    return true;
}

bool PocketDB::InitDB(std::string table)
{
    // Service
    if (table == "Service" || table == "ALL") {
        db->OpenNamespace("Service", StorageOpts().Enabled().CreateIfMissing());
        db->AddIndex("Service", {"version", "tree", "int", IndexOpts().PK()});
        db->Commit("Service");
    }

	// RI Mempool
    if (table == "Mempool" || table == "ALL") {
        db->OpenNamespace("Mempool", StorageOpts().Enabled().CreateIfMissing());
        db->AddIndex("Mempool", {"txid", "hash", "string", IndexOpts().PK()});
        db->AddIndex("Mempool", {"table", "-", "string", IndexOpts()});
        db->AddIndex("Mempool", {"data", "-", "string", IndexOpts()});
        db->Commit("Mempool");
    }

    // Users
    if (table == "User" || table == "ALL") {
        db->OpenNamespace("User", StorageOpts().Enabled().CreateIfMissing());
        db->AddIndex("User", {"txid", "hash", "string", IndexOpts().PK()});
        db->AddIndex("User", {"last", "-", "bool", IndexOpts()});
        db->AddIndex("User", {"block", "tree", "int", IndexOpts()});
        db->AddIndex("User", {"time", "-", "int64", IndexOpts()});
        db->AddIndex("User", {"address", "hash", "string", IndexOpts()});
        db->AddIndex("User", {"name", "hash", "string", IndexOpts().SetCollateMode(CollateUTF8)});
        db->AddIndex("User", {"birthday", "-", "int", IndexOpts()});
        db->AddIndex("User", {"gender", "-", "int", IndexOpts()});
        db->AddIndex("User", {"regdate", "-", "int64", IndexOpts()});
        db->AddIndex("User", {"avatar", "-", "string", IndexOpts()});
        db->AddIndex("User", {"about", "-", "string", IndexOpts().SetCollateMode(CollateUTF8)});
        db->AddIndex("User", {"lang", "-", "string", IndexOpts().SetCollateMode(CollateUTF8)});
        db->AddIndex("User", {"url", "-", "string", IndexOpts().SetCollateMode(CollateUTF8)});
        db->AddIndex("User", {"pubkey", "-", "string", IndexOpts()});
        db->AddIndex("User", {"donations", "-", "string", IndexOpts()});
        db->AddIndex("User", {"referrer", "hash", "string", IndexOpts()});
        db->AddIndex("User", {"id", "-", "int", IndexOpts()});
        db->AddIndex("User", {"reputation", "-", "int", IndexOpts()});
        db->AddIndex("User", {"name_text", {"name"}, "text", "composite", IndexOpts().SetCollateMode(CollateUTF8) });
        db->Commit("User");
    }

    // UserRatings
    if (table == "UserRatings" || table == "ALL") {
        db->OpenNamespace("UserRatings", StorageOpts().Enabled().CreateIfMissing());
        db->AddIndex("UserRatings", {"block", "tree", "int", IndexOpts()});
        db->AddIndex("UserRatings", {"address", "hash", "string", IndexOpts()});
        db->AddIndex("UserRatings", {"reputation", "-", "int", IndexOpts()});
        db->AddIndex("UserRatings", {"address+block", {"address", "block"}, "hash", "composite", IndexOpts().PK()});
        db->Commit("UserRatings");
    }

    // Posts
    if (table == "Post" || table == "ALL") {
        db->OpenNamespace("Post", StorageOpts().Enabled().CreateIfMissing());
        db->AddIndex("Post", {"txid", "hash", "string", IndexOpts().PK()});
        db->AddIndex("Post", {"otxid", "hash", "string", IndexOpts()});
        db->AddIndex("Post", {"block", "tree", "int", IndexOpts()});
        db->AddIndex("Post", {"time", "tree", "int64", IndexOpts()});
        db->AddIndex("Post", {"address", "hash", "string", IndexOpts()});
        db->AddIndex("Post", {"type", "tree", "int", IndexOpts()});
        db->AddIndex("Post", {"lang", "-", "string", IndexOpts()});
        db->AddIndex("Post", {"caption", "-", "string", IndexOpts().SetCollateMode(CollateUTF8)});
        db->AddIndex("Post", {"caption_", "-", "string", IndexOpts().SetCollateMode(CollateUTF8)});
        db->AddIndex("Post", {"message", "-", "string", IndexOpts().SetCollateMode(CollateUTF8)});
        db->AddIndex("Post", {"message_", "-", "string", IndexOpts().SetCollateMode(CollateUTF8)});
        db->AddIndex("Post", {"tags", "-", "string", IndexOpts().Array().SetCollateMode(CollateUTF8)});
        db->AddIndex("Post", {"url", "-", "string", IndexOpts().SetCollateMode(CollateUTF8)});
        db->AddIndex("Post", {"images", "-", "string", IndexOpts().Array().SetCollateMode(CollateUTF8)});
        db->AddIndex("Post", {"settings", "-", "string", IndexOpts().SetCollateMode(CollateUTF8)});
        db->AddIndex("Post", {"scoreSum", "-", "int", IndexOpts()});
        db->AddIndex("Post", {"scoreCnt", "-", "int", IndexOpts()});
        db->AddIndex("Post", {"reputation", "-", "int", IndexOpts()});
        db->AddIndex("Post", {"caption+message", {"caption_", "message_"}, "text", "composite", IndexOpts().SetCollateMode(CollateUTF8)});
        db->Commit("Post");
    }

    // PostRatings
    if (table == "PostRatings" || table == "ALL") {
        db->OpenNamespace("PostRatings", StorageOpts().Enabled().CreateIfMissing());
        db->AddIndex("PostRatings", {"block", "tree", "int", IndexOpts()});
        db->AddIndex("PostRatings", {"posttxid", "hash", "string", IndexOpts()});
        db->AddIndex("PostRatings", {"scoreSum", "-", "int", IndexOpts()});
        db->AddIndex("PostRatings", {"scoreCnt", "-", "int", IndexOpts()});
        db->AddIndex("PostRatings", {"reputation", "-", "int", IndexOpts()});
        db->AddIndex("PostRatings", {"posttxid+block", {"posttxid", "block"}, "hash", "composite", IndexOpts().PK()});
        db->Commit("PostRatings");
    }

    // Scores
    if (table == "Scores" || table == "ALL") {
        db->OpenNamespace("Scores", StorageOpts().Enabled().CreateIfMissing());
        db->AddIndex("Scores", {"txid", "hash", "string", IndexOpts().PK()});
        db->AddIndex("Scores", {"block", "tree", "int", IndexOpts()});
        db->AddIndex("Scores", {"time", "tree", "int64", IndexOpts()});
        db->AddIndex("Scores", {"posttxid", "hash", "string", IndexOpts()});
        db->AddIndex("Scores", {"address", "hash", "string", IndexOpts()});
        db->AddIndex("Scores", {"value", "-", "int", IndexOpts()});
        db->Commit("Scores");
    }

    // Subscribes
    if (table == "SubscribesView" || table == "ALL") {
        db->OpenNamespace("SubscribesView", StorageOpts().Enabled().CreateIfMissing());
        db->AddIndex("SubscribesView", {"txid", "hash", "string", IndexOpts().PK()});
        db->AddIndex("SubscribesView", {"block", "tree", "int", IndexOpts()});
        db->AddIndex("SubscribesView", {"time", "tree", "int64", IndexOpts()});
        db->AddIndex("SubscribesView", {"address", "hash", "string", IndexOpts()});
        db->AddIndex("SubscribesView", {"address_to", "hash", "string", IndexOpts()});
        db->AddIndex("SubscribesView", {"private", "-", "bool", IndexOpts()});
        db->Commit("SubscribesView");
    }

    // RI SubscribesHistory
    if (table == "Subscribes" || table == "ALL") {
        db->OpenNamespace("Subscribes", StorageOpts().Enabled().CreateIfMissing());
        db->AddIndex("Subscribes", {"txid", "hash", "string", IndexOpts().PK()});
        db->AddIndex("Subscribes", {"block", "tree", "int", IndexOpts()});
        db->AddIndex("Subscribes", {"time", "tree", "int64", IndexOpts()});
        db->AddIndex("Subscribes", {"address", "hash", "string", IndexOpts()});
        db->AddIndex("Subscribes", {"address_to", "hash", "string", IndexOpts()});
        db->AddIndex("Subscribes", {"private", "-", "bool", IndexOpts()});
        db->AddIndex("Subscribes", {"unsubscribe", "-", "bool", IndexOpts()});
        db->Commit("Subscribes");
    }

	// Blocking
    if (table == "BlockingView" || table == "ALL") {
        db->OpenNamespace("BlockingView", StorageOpts().Enabled().CreateIfMissing());
        db->AddIndex("BlockingView", {"txid", "hash", "string", IndexOpts().PK()});
        db->AddIndex("BlockingView", {"block", "tree", "int", IndexOpts()});
        db->AddIndex("BlockingView", {"time", "tree", "int64", IndexOpts()});
        db->AddIndex("BlockingView", {"address", "hash", "string", IndexOpts()});
        db->AddIndex("BlockingView", {"address_to", "hash", "string", IndexOpts()});
        db->AddIndex("BlockingView", {"address_reputation", "-", "int", IndexOpts()});
        db->Commit("BlockingView");
    }

	// BlockingHistory
    if (table == "Blocking" || table == "ALL") {
        db->OpenNamespace("Blocking", StorageOpts().Enabled().CreateIfMissing());
        db->AddIndex("Blocking", {"txid", "hash", "string", IndexOpts().PK()});
        db->AddIndex("Blocking", {"block", "tree", "int", IndexOpts()});
        db->AddIndex("Blocking", {"time", "tree", "int64", IndexOpts()});
        db->AddIndex("Blocking", {"address", "hash", "string", IndexOpts()});
        db->AddIndex("Blocking", {"address_to", "hash", "string", IndexOpts()});
        db->AddIndex("Blocking", {"unblocking", "-", "bool", IndexOpts()});
        db->Commit("Blocking");
    }

    // Complains
    if (table == "Complains" || table == "ALL") {
        db->OpenNamespace("Complains", StorageOpts().Enabled().CreateIfMissing());
        db->AddIndex("Complains", {"txid", "hash", "string", IndexOpts().PK()});
        db->AddIndex("Complains", {"block", "tree", "int", IndexOpts()});
        db->AddIndex("Complains", {"time", "tree", "int64", IndexOpts()});
        db->AddIndex("Complains", {"posttxid", "hash", "string", IndexOpts()});
        db->AddIndex("Complains", {"address", "hash", "string", IndexOpts()});
        db->AddIndex("Complains", {"reason", "-", "int", IndexOpts()});
        db->Commit("Complains");
    }

    // UTXO
    if (table == "UTXO" || table == "ALL") {
        db->OpenNamespace("UTXO", StorageOpts().Enabled().CreateIfMissing());
        db->AddIndex("UTXO", {"txid", "-", "string", IndexOpts()});
        db->AddIndex("UTXO", {"txout", "-", "int", IndexOpts()});
        db->AddIndex("UTXO", {"time", "-", "int64", IndexOpts()});
        db->AddIndex("UTXO", {"block", "tree", "int", IndexOpts()});
        db->AddIndex("UTXO", {"address", "hash", "string", IndexOpts()});
        db->AddIndex("UTXO", {"amount", "-", "int64", IndexOpts()});
        db->AddIndex("UTXO", {"spent_block", "tree", "int", IndexOpts()});
        db->AddIndex("UTXO", {"txid+txout", {"txid", "txout"}, "hash", "composite", IndexOpts().PK()});
        db->Commit("UTXO");
    }

    // Addresses
    if (table == "Addresses" || table == "ALL") {
        db->OpenNamespace("Addresses", StorageOpts().Enabled().CreateIfMissing());
        db->AddIndex("Addresses", {"txid", "hash", "string", IndexOpts()});
        db->AddIndex("Addresses", {"block", "tree", "int", IndexOpts()});
        db->AddIndex("Addresses", {"address", "hash", "string", IndexOpts().PK()});
        db->AddIndex("Addresses", {"time", "tree", "int64", IndexOpts()});
        db->Commit("Addresses");
    }

    // Comment
    if (table == "Comment" || table == "ALL") {
        db->OpenNamespace("Comment", StorageOpts().Enabled().CreateIfMissing());
        db->AddIndex("Comment", {"txid", "hash", "string", IndexOpts().PK()});
        db->AddIndex("Comment", {"otxid", "hash", "string", IndexOpts()});
        db->AddIndex("Comment", {"last", "-", "bool", IndexOpts()});
        db->AddIndex("Comment", {"postid", "hash", "string", IndexOpts()});
        db->AddIndex("Comment", {"address", "hash", "string", IndexOpts()});
        db->AddIndex("Comment", {"time", "tree", "int64", IndexOpts()});
        db->AddIndex("Comment", {"block", "tree", "int", IndexOpts()});
        db->AddIndex("Comment", {"msg", "-", "string", IndexOpts().SetCollateMode(CollateUTF8)});
        db->AddIndex("Comment", {"parentid", "hash", "string", IndexOpts()});
        db->AddIndex("Comment", {"answerid", "hash", "string", IndexOpts()});
        db->AddIndex("Comment", {"scoreUp", "-", "int", IndexOpts()});
        db->AddIndex("Comment", {"scoreDown", "-", "int", IndexOpts()});
        db->AddIndex("Comment", {"reputation", "-", "int", IndexOpts()});
        db->Commit("Comment");
    }

    // CommentRatings
    if (table == "CommentRatings" || table == "ALL") {
        db->OpenNamespace("CommentRatings", StorageOpts().Enabled().CreateIfMissing());
        db->AddIndex("CommentRatings", {"block", "tree", "int", IndexOpts()});
        db->AddIndex("CommentRatings", {"commentid", "hash", "string", IndexOpts()});
        db->AddIndex("CommentRatings", {"scoreUp", "-", "int", IndexOpts()});
        db->AddIndex("CommentRatings", {"scoreDown", "-", "int", IndexOpts()});
        db->AddIndex("CommentRatings", {"reputation", "-", "int", IndexOpts()});
        db->AddIndex("CommentRatings", {"commentid+block", {"commentid", "block"}, "hash", "composite", IndexOpts().PK()});
        db->Commit("CommentRatings");
    }

    // CommentScores
    if (table == "CommentScores" || table == "ALL") {
        db->OpenNamespace("CommentScores", StorageOpts().Enabled().CreateIfMissing());
        db->AddIndex("CommentScores", {"txid", "hash", "string", IndexOpts().PK()});
        db->AddIndex("CommentScores", {"block", "tree", "int", IndexOpts()});
        db->AddIndex("CommentScores", {"time", "tree", "int64", IndexOpts()});
        db->AddIndex("CommentScores", {"commentid", "hash", "string", IndexOpts()});
        db->AddIndex("CommentScores", {"address", "hash", "string", IndexOpts()});
        db->AddIndex("CommentScores", {"value", "-", "int", IndexOpts()});
        db->Commit("CommentScores");
    }

    return true;
}

bool PocketDB::DropTable(std::string table)
{
    Error err = db->DropNamespace(table).ok();
    if (!InitDB(table)) return false;
    return true;
}

bool PocketDB::CheckIndexes(UniValue& obj)
{
    Error err;
    bool ret = true;
    std::vector<NamespaceDef> nss;
    db->EnumNamespaces(nss, false);

    for (NamespaceDef& ns : nss) {
        if (std::find_if(ns.indexes.begin(), ns.indexes.end(), [&](const IndexDef& idx) { return idx.name_ == "txid"; }) == ns.indexes.end()) {
            continue;
        }
        //--------------------------
        QueryResults _res;
        err = db->Select(Query(ns.name).ReqTotal(), _res);
        ret = ret && err.ok();
        //--------------------------
        if (err.ok()) {
            UniValue tbl(UniValue::VARR);
            for (auto& it : _res) {
                Item itm(it.GetItem());
                tbl.push_back(itm["txid"].As<string>());
            }
            obj.pushKV(ns.name, tbl);
        }
    }
    //--------------------------
    return ret;
}

bool PocketDB::GetStatistic(std::string table, UniValue& obj)
{
    Error err;
    if (table != "") {
        QueryResults _res;
        err = db->Select(reindexer::Query(table).Sort("block", true).ReqTotal(), _res);
        if (err.ok()) {
            obj.pushKV("total", (uint64_t)_res.TotalCount());

            UniValue t_arr(UniValue::VARR);
            for (auto& it : _res) {
                t_arr.push_back(it.GetItem().GetJSON().ToString());
            }
            obj.pushKV("items", t_arr);
        }

        return err.ok();
    }
    //--------------------------
    bool ret = true;
    std::vector<NamespaceDef> nss;
    db->EnumNamespaces(nss, false);

    struct NamespaceDefSortComp
    {
        inline bool operator() (const NamespaceDef& ns1, const NamespaceDef& ns2)
        {
            return ns1.name < ns2.name;
        }
    };

    std::sort(nss.begin(), nss.end(), NamespaceDefSortComp());

    for (NamespaceDef& ns : nss) {
        if (ns.name[0] == '#') continue;
        //--------------------------
        QueryResults _res;
        err = db->Select(reindexer::Query(ns.name).ReqTotal(), _res);
        ret = ret && err.ok();
        if (err.ok()) obj.pushKV(ns.name, (uint64_t)_res.TotalCount());
    }

    return ret;
}

// -------------------------------
//             SQL
// -------------------------------

bool PocketDB::Exists(Query query)
{
    Item _itm;
    return SelectOne(query, _itm).ok();
}

size_t PocketDB::SelectTotalCount(std::string table)
{
    Error err;
    QueryResults _res;
    err = db->Select(Query(table).ReqTotal(), _res);
    if (err.ok())
        return _res.TotalCount();
    else
        return 0;
}

size_t PocketDB::SelectCount(Query query)
{
    // TODO (brangr): Its not funny! :D
    QueryResults _res;
    if (db->Select(query, _res).ok())
        return _res.Count();
    else
        return 0;
}

Error PocketDB::Select(Query query, QueryResults& res)
{
    reindexer::Query _query(query);
    return db->Select(_query, res);
}

Error PocketDB::SelectOne(Query query, Item& item)
{
    reindexer::Query _query(query);
    _query.start = 0;
    _query.count = 1;
    QueryResults res;
    Error err = db->Select(_query, res);
    if (err.ok()) {
        if (res.Count() > 0) {
            item = res[0].GetItem();
        } else {
            return Error(13);
        }
    }

    return err;
}

Error PocketDB::SelectAggr(Query query, QueryResults& aggRes)
{
    Error err = db->Select(query, aggRes);
    if (err.ok()) {
        if (aggRes.aggregationResults.size() > 0) {
            return err;
        } else {
            return Error(13);
        }
    }

    return err;
}

Error PocketDB::SelectAggr(Query query, std::string aggId, AggregationResult& aggRes)
{
    QueryResults res;
    Error err = db->Select(query, res);
    if (err.ok()) {
        if (res.aggregationResults.size() > 0) {
            aggRes = std::find_if(res.aggregationResults.begin(), res.aggregationResults.end(),
                [&](const reindexer::AggregationResult& agg) { return agg.name == aggId; })[0];
        } else {
            return Error(13);
        }
    }

    return err;
}

Error PocketDB::Upsert(std::string table, Item& item)
{
    return db->Upsert(table, item);
}

Error PocketDB::UpsertWithCommit(std::string table, Item& item)
{
    Error err = db->Upsert(table, item);
    if (err.ok()) return db->Commit(table);
    return err;
}

Error PocketDB::Delete(Query query)
{
    QueryResults res;
    Error err = db->Delete(query, res);

    return err;
}

Error PocketDB::DeleteWithCommit(Query query)
{
    QueryResults res;
    Error err = db->Delete(query, res);

    if (err.ok()) {
        return db->Commit(query._namespace);
    }

    return err;
}

Error PocketDB::Update(std::string table, Item& item, bool commit)
{
    Error err = db->Update(table, item);
    if (err.ok() && commit) return db->Commit(table);
    return err;
}

Error PocketDB::UpdateSubscribesView(std::string address, std::string address_to)
{
    QueryResults _res;
    Error err = db->Select(Query("Subscribes", 0, 1).Where("address", CondEq, address).Where("address_to", CondEq, address_to).Sort("time", true), _res);
    if (_res.Count() <= 0) return DeleteWithCommit(Query("SubscribesView").Where("address", CondEq, address).Where("address_to", CondEq, address_to));
    if (err.ok()) {
        Item _itm = _res[0].GetItem();
        if (_itm["unsubscribe"].As<bool>() == true)
            return DeleteWithCommit(Query("SubscribesView").Where("address", CondEq, address).Where("address_to", CondEq, address_to));
        else
            return UpsertWithCommit("SubscribesView", _itm);
    }
    //-----------------
    return err;
}

Error PocketDB::UpdateBlockingView(std::string address, std::string address_to)
{
    Item _blocking_itm;
    Error err = SelectOne(Query("Blocking").Where("address", CondEq, address).Where("address_to", CondEq, address_to).Sort("time", true), _blocking_itm);
    if (err.code() == 13) return DeleteWithCommit(Query("BlockingView").Where("address", CondEq, address).Where("address_to", CondEq, address_to));
    if (err.ok()) {
        if (_blocking_itm["unblocking"].As<bool>() == true) {
            return DeleteWithCommit(Query("BlockingView").Where("address", CondEq, address).Where("address_to", CondEq, address_to));
        }
        else {
            Item _blocking_view_itm = db->NewItem("BlockingView");
            _blocking_view_itm["txid"] = _blocking_itm["txid"].As<string>();
            _blocking_view_itm["block"] = _blocking_itm["block"].As<int>();
            _blocking_view_itm["time"] = _blocking_itm["time"].As<int64_t>();
            _blocking_view_itm["address"] = _blocking_itm["address"].As<string>();
            _blocking_view_itm["address_to"] = _blocking_itm["address_to"].As<string>();
            int rep = 0;
            GetUserReputation(address, _blocking_itm["block"].As<int>(), rep);
            _blocking_view_itm["address_reputation"] = rep;
                
            return UpsertWithCommit("BlockingView", _blocking_view_itm);
        }
    }

    return err;
}

// -------------------------------
//            USERS
// -------------------------------

void PocketDB::GetUserBalance(std::string _address, int height, int64_t& balance)
{
    balance = 0;
    AggregationResult aggRes;
    if (SelectAggr(
        Query("UTXO")
        .Where("address", CondEq, _address)
        .Where("block", CondLt, height)
        .Where("spent_block", CondEq, 0)
        .Aggregate("amount", AggSum)
        ,"amount"
        ,aggRes
    ).ok()) {
        balance = (int64_t)aggRes.value;
    }
}

void PocketDB::GetUserReputation(std::string _address, int height, int& rep)
{
    rep = 0;

    // Sorting by block desc - last accumulating rating
    Item _itm_rating;
    if (SelectOne(
            Query("UserRatings")
            .Where("address", CondEq, _address)
            .Where("block", CondLe, height)
            .Sort("block", true)
            , _itm_rating
        ).ok()
    ) {
        rep = _itm_rating["reputation"].As<int>();
    }
}

bool PocketDB::SetUserReputation(std::string address, int rep)
{
    reindexer::QueryResults userViewRes;
    if (!db->Select(reindexer::Query("User", 0, 1).Where("last", CondEq, true).Where("address", CondEq, address), userViewRes).ok()) return false;
    for (auto& r : userViewRes) {
        reindexer::Item userItm(r.GetItem());
        userItm["reputation"] = rep;
        if (!UpsertWithCommit("User", userItm).ok()) return false;
    }

    return true;
}

bool PocketDB::UpdateUserReputation(std::string address, int height)
{
    int rep = 0;
    GetUserReputation(address, height, rep);
    return SetUserReputation(address, rep);
}

Error PocketDB::CommitLastUserItem(Item& itm, int height, bool disable_old) {
    Error err;

    // Disable all founded last items
    if (disable_old) {
        
        QueryResults all_res;
        err = db->Select(Query("User").Where("address", CondEq, itm["address"].As<string>()).Where("last", CondEq, true), all_res);
        if (!err.ok()) return err;

        if (all_res.Count() > 0) {

            for (auto& it : all_res) {
                Item _itm = it.GetItem();
                _itm["last"] = false;
                _itm["reputation"] = 0;
                err = UpsertWithCommit("User", _itm);
                if (!err.ok()) return err;

                // Copy parameters from exists record
                itm["id"] = _itm["id"].As<int>();
                itm["regdate"] = _itm["regdate"].As<int64_t>();
                itm["referrer"] = _itm["referrer"].As<string>();
            }

        } else {
            // Default parameters for new user
            itm["id"] = (int)g_pocketdb->SelectCount(Query("User").Where("last", CondEq, true));
            itm["regdate"] = itm["time"].As<int64_t>();
        }

    }

    // Restore reputation
    int rep = 0;
    GetUserReputation(itm["address"].As<string>(), height, rep);
    itm["reputation"] = rep;

    // This item as last
    itm["last"] = true;

    err = UpsertWithCommit("User", itm);
    return err;
}

Error PocketDB::RestoreLastUserItem(std::string txid, std::string address, int height) {

    // delete last by txid
    Error err = DeleteWithCommit(Query("User").Where("txid", CondEq, txid));
    if (!err.ok()) return err;

    // select last
    QueryResults last_res;
    err = db->Select(Query("User", 0, 1).Where("address", CondEq, address).Sort("block", true), last_res);
    if (err.ok()) {
        if (last_res.Count() > 0) {
            Item last_item = last_res[0].GetItem();
            return CommitLastUserItem(last_item, height, false);
        } else {
            return Error(errOK);
        }
    } else {
        return err;
    }

}


// -------------------------------
//             POSTS
// -------------------------------

void PocketDB::GetPostRating(std::string posttxid, int& sum, int& cnt, int& rep, int height)
{
    // Set to default if rating for post not found
    sum = 0;
    cnt = 0;
    rep = 0;

    // Sorting by block desc - last accumulating rating
    Item _itm_rating_cur;
    if (SelectOne(
            Query("PostRatings")
            .Where("posttxid", CondEq, posttxid)
            .Where("block", CondLe, height)
            .Sort("block", true)
            , _itm_rating_cur
        ).ok()
    ) {
        sum = _itm_rating_cur["scoreSum"].As<int>();
        cnt = _itm_rating_cur["scoreCnt"].As<int>();
        rep = _itm_rating_cur["reputation"].As<int>();
    }
}

bool PocketDB::UpdatePostRating(std::string posttxid, int sum, int cnt, int& rep)
{
    reindexer::QueryResults postsRes;
    if (!db->Select(reindexer::Query("Post", 0, 1).Where("otxid", CondEq, posttxid).Where("last", CondEq, true), postsRes).ok()) return false;
    for (auto& p : postsRes) {
        reindexer::Item postItm(p.GetItem());
        postItm["scoreSum"] = sum;
        postItm["scoreCnt"] = cnt;
        postItm["reputation"] = rep;
        if (!UpsertWithCommit("Post", postItm).ok()) return false;
    }

    return true;
}

bool PocketDB::UpdatePostRating(std::string posttxid, int height)
{
    int sum = 0;
    int cnt = 0;
    int rep = 0;
    GetPostRating(posttxid, sum, cnt, rep, height);

    return UpdatePostRating(posttxid, sum, cnt, rep);
}

Error PocketDB::CommitLastPostItem(Item& itm, int height, bool disable_old) {
    Error err;

    // Disable all founded last items
    if (disable_old) {
        QueryResults all_res;
        err = db->Select(Query("Post").Where("otxid", CondEq, itm["otxid"].As<string>()).Where("last", CondEq, true), all_res);
        if (!err.ok()) return err;
        for (auto& it : all_res) {
            Item _itm = it.GetItem();
            _itm["last"] = false;
            _itm["scoreSum"] = 0;
            _itm["scoreCnt"] = 0;
            _itm["reputation"] = 0;
            _itm["caption_"] = "";
            _itm["message_"] = "";
            err = UpsertWithCommit("Post", _itm);
            if (!err.ok()) return err;
        }
    }

    // Restore rating
    int sum = 0;
    int cnt = 0;
    int rep = 0;
    GetPostRating(itm["otxid"].As<string>(), sum, cnt, rep, height);
    itm["scoreSum"] = sum;
    itm["scoreCnt"] = cnt;
    itm["reputation"] = rep;

    // Clear data for searching
    std::string caption_decoded = UrlDecode(itm["caption"].As<string>());
    std::string message_decoded = UrlDecode(itm["message"].As<string>());
    itm["caption_"] = ClearHtmlTags(caption_decoded);
    itm["message_"] = ClearHtmlTags(message_decoded);

    // Insert new item
    itm["last"] = true;
    err = UpsertWithCommit("Post", itm);
    return err;
}

Error PocketDB::RestoreLastPostItem(std::string txid, std::string otxid, int height) {

    // delete last by txid
    Error err = DeleteWithCommit(Query("Post").Where("txid", CondEq, txid));
    if (!err.ok()) return err;

    // select last
    QueryResults last_res;
    err = db->Select(Query("Post", 0, 1).Where("otxid", CondEq, otxid).Sort("block", true), last_res);
    if (err.ok()) {
        if (last_res.Count() > 0) {
            Item last_item = last_res[0].GetItem();
            return CommitLastPostItem(last_item, height, false);

        } else {
            return Error(errOK);
        }
    } else {
        return err;
    }

}

// -------------------------------
//           COMMENTS
// -------------------------------

void PocketDB::GetCommentRating(std::string commentid, int& up, int& down, int& rep, int height)
{
    // Set to default if rating for post not found
    up = 0;
    down = 0;
    rep = 0;

    // Sorting by block desc - last accumulating rating
    Item _itm_rating_cur;
    if (SelectOne(
            Query("CommentRatings")
            .Where("commentid", CondEq, commentid)
            .Where("block", CondLe, height)
            .Sort("block", true)
            , _itm_rating_cur
        ).ok()
    ) {
        up = _itm_rating_cur["scoreUp"].As<int>();
        down = _itm_rating_cur["scoreDown"].As<int>();
        rep = _itm_rating_cur["reputation"].As<int>();
    }
}

bool PocketDB::UpdateCommentRating(std::string commentid, int up, int down, int rep)
{
    reindexer::QueryResults commentRes;
    if (!db->Select(reindexer::Query("Comment", 0, 1).Where("otxid", CondEq, commentid).Where("last", CondEq, true), commentRes).ok()) return false;
    for (auto& p : commentRes) {
        reindexer::Item postItm(p.GetItem());
        postItm["scoreUp"] = up;
        postItm["scoreDown"] = down;
        postItm["reputation"] = rep;
        if (!UpsertWithCommit("Comment", postItm).ok()) return false;
    }

    return true;
}

bool PocketDB::UpdateCommentRating(std::string commentid, int height)
{
    int up = 0;
    int down = 0;
    int rep = 0;
    GetCommentRating(commentid, up, down, rep, height);
    return UpdateCommentRating(commentid, up, down, rep);
}

Error PocketDB::CommitLastCommentItem(Item& itm, int height, bool disable_old) {
    Error err;

    // Disable all founded last items
    if (disable_old) {
        QueryResults all_res;
        err = db->Select(Query("Comment").Where("otxid", CondEq, itm["otxid"].As<string>()).Where("last", CondEq, true), all_res);
        if (!err.ok()) return err;
        for (auto& it : all_res) {
            Item _itm = it.GetItem();
            _itm["last"] = false;
            _itm["scoreUp"] = 0;
            _itm["scoreDown"] = 0;
            _itm["reputation"] = 0;
            err = UpsertWithCommit("Comment", _itm);
            if (!err.ok()) return err;
        }
    }

    // Restore rating
    int up = 0;
    int down = 0;
    int rep = 0;
    GetCommentRating(itm["otxid"].As<string>(), up, down, rep, height);
    itm["scoreUp"] = up;
    itm["scoreDown"] = down;
    itm["reputation"] = rep;

    // Insert new item
    itm["last"] = true;
    err = UpsertWithCommit("Comment", itm);
    return err;
}

Error PocketDB::RestoreLastCommentItem(std::string txid, std::string otxid, int height) {

    // delete last by txid
    Error err = DeleteWithCommit(Query("Comment").Where("txid", CondEq, txid));
    if (!err.ok()) return err;

    // select last
    QueryResults last_res;
    err = db->Select(Query("Comment", 0, 1).Where("otxid", CondEq, otxid).Sort("block", true), last_res);
    if (err.ok()) {
        if (last_res.Count() > 0) {
            Item last_item = last_res[0].GetItem();
            return CommitLastCommentItem(last_item, height, false);

        } else {
            return Error(errOK);
        }
    } else {
        return err;
    }

}

// -------------------------------
//             OTHER
// -------------------------------

void PocketDB::SearchTags(std::string search, int count, std::map<std::string, int>& tags, int& totalCount)
{
    if (search.size() < 3) return;
    int _count = (count > 1000 ? 1000 : count);

    reindexer::QueryResults res;
    if (g_pocketdb->Select(reindexer::Query("Tags", 0, _count).Where("tag", CondEq, (search.size() > 0 ? search : "*")), res).ok()) {
        totalCount = res.TotalCount();
        for (auto& it : res) {
            Item _itm = it.GetItem();
            tags.insert_or_assign(_itm["tag"].As<string>(), _itm["rating"].As<int>());
        }
    }
}

bool PocketDB::GetHashItem(Item& item, std::string table, bool with_referrer, std::string& out_hash)
{
    std::string data = "";
    //------------------------
    if (table == "Post") {
        // self.url.v + self.caption.v + self.message.v + self.tags.v.join(',') + self.images.v.join(',') + self.otxid.v
        data += item["url"].As<string>();
        data += item["caption"].As<string>();
        data += item["message"].As<string>();

        reindexer::VariantArray va_tags = item["tags"];
        for (size_t i = 0; i < va_tags.size(); ++i)
            data += (i ? "," : "") + va_tags[i].As<string>();

        reindexer::VariantArray va_images = item["images"];
        for (size_t i = 0; i < va_images.size(); ++i)
            data += (i ? "," : "") + va_images[i].As<string>();

        data += item["txid"].As<string>();
    }
    
    if (table == "Scores") {
        // self.share.v + self.value.v
        data += item["posttxid"].As<string>();
        data += std::to_string(item["value"].As<int>());
    }
    
    if (table == "Complains") {
        // self.share.v + '_' + self.reason.v
        data += item["posttxid"].As<string>();
        data += "_";
        data += std::to_string(item["reason"].As<int>());
    }
    
    if (table == "Subscribes") {
        // self.address.v
        data += item["address_to"].As<string>();
    }
    
    if (table == "Blocking") {
        // self.address.v
        data += item["address_to"].As<string>();
    }
    
    if (table == "User") {
        // self.name.v + self.site.v + self.language.v + self.about.v + self.image.v + JSON.stringify(self.addresses.v)
        data += item["name"].As<string>();
        data += item["url"].As<string>();
        data += item["lang"].As<string>();
        data += item["about"].As<string>();
        data += item["avatar"].As<string>();
        data += item["donations"].As<string>();
        if (with_referrer) data += item["referrer"].As<string>();
        data += item["pubkey"].As<string>();
    }

    if (table == "Comment") {
        data += item["postid"].As<string>();
        data += item["msg"].As<string>();
        data += item["parentid"].As<string>();
        data += item["answerid"].As<string>();
    }
     
    if (table == "CommentScores") {
        data += item["commentid"].As<string>();
        data += std::to_string(item["value"].As<int>());
    }
    //------------------------
    // Compute hash for serialized item data
    unsigned char hash[32] = {};
    CSHA256().Write((const unsigned char*)data.data(), data.size()).Finalize(hash);
    CSHA256().Write(hash, 32).Finalize(hash);

    std::vector<unsigned char> vec(hash, hash + sizeof(hash));
    out_hash = HexStr(vec);
    return true;
}