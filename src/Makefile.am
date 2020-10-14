# Copyright (c) 2013-2016 The Pocketcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

DIST_SUBDIRS = secp256k1 univalue

AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS) $(GPROF_LDFLAGS) $(SANITIZER_LDFLAGS)
AM_CXXFLAGS = $(DEBUG_CXXFLAGS) $(HARDENED_CXXFLAGS) $(WARN_CXXFLAGS) $(NOWARN_CXXFLAGS) $(ERROR_CXXFLAGS) $(GPROF_CXXFLAGS) $(SANITIZER_CXXFLAGS)
AM_CPPFLAGS = $(DEBUG_CPPFLAGS) $(HARDENED_CPPFLAGS)
AM_LIBTOOLFLAGS = --preserve-dup-deps
EXTRA_LIBRARIES =

if EMBEDDED_UNIVALUE
LIBUNIVALUE = univalue/libunivalue.la

$(LIBUNIVALUE): $(wildcard univalue/lib/*) $(wildcard univalue/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)
else
LIBUNIVALUE = $(UNIVALUE_LIBS)
endif

REINDEXER_INCLUDES = -Ileveldb/include -Ireindexer -Ireindexer/vendor -Ireindexer/vendor/msgpack
POCKETCOIN_INCLUDES=-I$(builddir) $(BDB_CPPFLAGS) $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

POCKETCOIN_INCLUDES += -I$(srcdir)/secp256k1/include
POCKETCOIN_INCLUDES += $(UNIVALUE_CFLAGS)
POCKETCOIN_INCLUDES += $(REINDEXER_INCLUDES)

LIBPOCKETCOIN_SERVER=libpocketcoin_server.a
LIBPOCKETCOIN_COMMON=libpocketcoin_common.a
LIBPOCKETCOIN_CONSENSUS=libpocketcoin_consensus.a
LIBPOCKETCOIN_CLI=libpocketcoin_cli.a
LIBPOCKETCOIN_UTIL=libpocketcoin_util.a
LIBPOCKETCOIN_CRYPTO_BASE=crypto/libpocketcoin_crypto_base.a
LIBPOCKETCOINQT=qt/libpocketcoinqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la
LIBREINDEXER=libreindexer.a

if ENABLE_ZMQ
LIBPOCKETCOIN_ZMQ=libpocketcoin_zmq.a
endif
if BUILD_POCKETCOIN_LIBS
LIBPOCKETCOINCONSENSUS=libpocketcoinconsensus.la
endif
if ENABLE_WALLET
LIBPOCKETCOIN_WALLET=libpocketcoin_wallet.a
endif

LIBPOCKETCOIN_CRYPTO= $(LIBPOCKETCOIN_CRYPTO_BASE)
if ENABLE_SSE41
LIBPOCKETCOIN_CRYPTO_SSE41 = crypto/libpocketcoin_crypto_sse41.a
LIBPOCKETCOIN_CRYPTO += $(LIBPOCKETCOIN_CRYPTO_SSE41)
endif
if ENABLE_AVX2
LIBPOCKETCOIN_CRYPTO_AVX2 = crypto/libpocketcoin_crypto_avx2.a
LIBPOCKETCOIN_CRYPTO += $(LIBPOCKETCOIN_CRYPTO_AVX2)
endif
if ENABLE_SHANI
LIBPOCKETCOIN_CRYPTO_SHANI = crypto/libpocketcoin_crypto_shani.a
LIBPOCKETCOIN_CRYPTO += $(LIBPOCKETCOIN_CRYPTO_SHANI)
endif

$(LIBSECP256K1): $(wildcard secp256k1/src/*.h) $(wildcard secp256k1/src/*.c) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES += \
  $(LIBREINDEXER) \
  $(LIBPOCKETCOIN_CRYPTO) \
  $(LIBPOCKETCOIN_UTIL) \
  $(LIBPOCKETCOIN_COMMON) \
  $(LIBPOCKETCOIN_CONSENSUS) \
  $(LIBPOCKETCOIN_SERVER) \
  $(LIBPOCKETCOIN_CLI) \
  $(LIBPOCKETCOIN_WALLET) \
  $(LIBPOCKETCOIN_ZMQ)  

lib_LTLIBRARIES = $(LIBPOCKETCOINCONSENSUS)

bin_PROGRAMS =
noinst_PROGRAMS =
TESTS =
BENCHMARKS =

if BUILD_POCKETCOIND
  bin_PROGRAMS += pocketcoind
endif

if BUILD_POCKETCOIN_UTILS
  bin_PROGRAMS += pocketcoin-cli pocketcoin-tx
endif

.PHONY: FORCE check-symbols check-security

# REINDEXER Headers
REINDEXER_H = \
    reindexer/client/internalrdxcontext.h \
    reindexer/client/item.h \
    reindexer/client/itemimpl.h \
    reindexer/client/namespace.h \
    reindexer/client/queryresults.h \
    reindexer/client/reindexer.h \
    reindexer/client/reindexerconfig.h \
    reindexer/client/resultserializer.h \
    reindexer/client/rpcclient.h \
    reindexer/client/rpcclientmock.h \
    reindexer/client/transaction.h \
    reindexer/core/cbinding/cgocancelcontextpool.h \
    reindexer/core/cbinding/reindexer_c.h \
    reindexer/core/cbinding/reindexer_ctypes.h \
    reindexer/core/cbinding/resultserializer.h \
    reindexer/core/cjson/baseencoder.h \
    reindexer/core/cjson/cjsonbuilder.h \
    reindexer/core/cjson/cjsondecoder.h \
    reindexer/core/cjson/cjsonmodifier.h \
    reindexer/core/cjson/cjsontools.h \
    reindexer/core/cjson/ctag.h \
    reindexer/core/cjson/fieldextractor.h \
    reindexer/core/cjson/jschemachecker.h \
    reindexer/core/cjson/jsonbuilder.h \
    reindexer/core/cjson/jsondecoder.h \
    reindexer/core/cjson/msgpackbuilder.h \
    reindexer/core/cjson/msgpackdecoder.h \
    reindexer/core/cjson/objtype.h \
    reindexer/core/cjson/tagslengths.h \
    reindexer/core/cjson/tagsmatcher.h \
    reindexer/core/cjson/tagsmatcherimpl.h \
    reindexer/core/cjson/tagspathcache.h \
    reindexer/core/ft/config/baseftconfig.h \
    reindexer/core/ft/config/ftfastconfig.h \
    reindexer/core/ft/config/ftfuzzyconfig.h \
    reindexer/core/ft/filters/itokenfilter.h \
    reindexer/core/ft/filters/kblayout.h \
    reindexer/core/ft/filters/synonyms.h \
    reindexer/core/ft/filters/translit.h \
    reindexer/core/ft/ft_fast/dataholder.h \
    reindexer/core/ft/ft_fast/dataprocessor.h \
    reindexer/core/ft/ft_fast/indextexttypes.h \
    reindexer/core/ft/ft_fast/selecter.h \
    reindexer/core/ft/ft_fuzzy/dataholder/basebuildedholder.h \
    reindexer/core/ft/ft_fuzzy/dataholder/datastruct.h \
    reindexer/core/ft/ft_fuzzy/dataholder/smardeque.h \
    reindexer/core/ft/ft_fuzzy/dumper/fulltextdumper.h \
    reindexer/core/ft/ft_fuzzy/merger/basemerger.h \
    reindexer/core/ft/ft_fuzzy/merger/dataset.h \
    reindexer/core/ft/ft_fuzzy/prefilter/prefilter.h \
    reindexer/core/ft/ft_fuzzy/advacedpackedvec.h \
    reindexer/core/ft/ft_fuzzy/baseseacher.h \
    reindexer/core/ft/ft_fuzzy/searchengine.h \
    reindexer/core/ft/stopwords/stop.h \
    reindexer/core/ft/areaholder.h \
    reindexer/core/ft/bm25.h \
    reindexer/core/ft/ftdsl.h \
    reindexer/core/ft/ftsetcashe.h \
    reindexer/core/ft/idrelset.h \
    reindexer/core/ft/numtotext.h \
    reindexer/core/ft/stemmer.h \
    reindexer/core/ft/typos.h \
    reindexer/core/index/indextext/fastindextext.h \
    reindexer/core/index/indextext/fieldsgetter.h \
    reindexer/core/index/indextext/ftkeyentry.h \
    reindexer/core/index/indextext/fuzzyindextext.h \
    reindexer/core/index/indextext/indextext.h \
    reindexer/core/index/index.h \
    reindexer/core/index/indexiterator.h \
    reindexer/core/index/indexordered.h \
    reindexer/core/index/indexstore.h \
    reindexer/core/index/indexunordered.h \
    reindexer/core/index/keyentry.h \
    reindexer/core/index/payload_map.h \
    reindexer/core/index/string_map.h \
    reindexer/core/index/ttlindex.h \
    reindexer/core/index/updatetracker.h \
    reindexer/core/keyvalue/key_string.h \
    reindexer/core/keyvalue/p_string.h \
    reindexer/core/keyvalue/variant.h \
    reindexer/core/namespace/namespace.h \
    reindexer/core/namespace/namespaceimpl.h \
    reindexer/core/namespace/namespacestat.h \
    reindexer/core/nsselecter/aggregator.h \
    reindexer/core/nsselecter/btreeindexiterator.h \
    reindexer/core/nsselecter/btreeindexiteratorimpl.h \
    reindexer/core/nsselecter/explaincalc.h \
    reindexer/core/nsselecter/itemcomparator.h \
    reindexer/core/nsselecter/joinedselector.h \
    reindexer/core/nsselecter/joinedselectormock.h \
    reindexer/core/nsselecter/nsselecter.h \
    reindexer/core/nsselecter/querypreprocessor.h \
    reindexer/core/nsselecter/selectiterator.h \
    reindexer/core/nsselecter/selectiteratorcontainer.h \
    reindexer/core/nsselecter/sortexpression.h \
    reindexer/core/nsselecter/sortingcontext.h \
    reindexer/core/payload/fieldsset.h \
    reindexer/core/payload/payloadfieldtype.h \
    reindexer/core/payload/payloadfieldvalue.h \
    reindexer/core/payload/payloadiface.h \
    reindexer/core/payload/payloadtype.h \
    reindexer/core/payload/payloadvalue.h \
    reindexer/core/query/dsl/dslencoder.h \
    reindexer/core/query/dsl/dslparser.h \
    reindexer/core/query/dsl/query.json.h \
    reindexer/core/query/sql/sqlencoder.h \
    reindexer/core/query/sql/sqlparser.h \
    reindexer/core/query/sql/sqlsuggester.h \
    reindexer/core/query/sql/sqltokentype.h \
    reindexer/core/query/expressionevaluator.h \
    reindexer/core/query/query.h \
    reindexer/core/query/queryentry.h \
    reindexer/core/queryresults/aggregationresult.h \
    reindexer/core/queryresults/itemref.h \
    reindexer/core/queryresults/joinresults.h \
    reindexer/core/queryresults/queryresults.h \
    reindexer/core/queryresults/tableviewbuilder.h \
    reindexer/core/selectfunc/ctx/basefunctionctx.h \
    reindexer/core/selectfunc/ctx/ftctx.h \
    reindexer/core/selectfunc/functions/highlight.h \
    reindexer/core/selectfunc/functions/snippet.h \
    reindexer/core/selectfunc/functionexecutor.h \
    reindexer/core/selectfunc/nsselectfuncinterface.h \
    reindexer/core/selectfunc/selectfunc.h \
    reindexer/core/selectfunc/selectfuncparser.h \
    reindexer/core/storage/basestorage.h \
    reindexer/core/storage/idatastorage.h \
    reindexer/core/storage/leveldbstorage.h \
    reindexer/core/storage/nsdirectoriesmap.h \
    reindexer/core/storage/rocksdbstorage.h \
    reindexer/core/storage/storagefactory.h \
    reindexer/core/storage/storagetype.h \
    reindexer/core/activity_context.h \
    reindexer/core/cancelcontextpool.h \
    reindexer/core/comparator.h \
    reindexer/core/comparatorimpl.h \
    reindexer/core/compositearraycomparator.h \
    reindexer/core/dbconfig.h \
    reindexer/core/expressiontree.h \
    reindexer/core/iclientsstats.h \
    reindexer/core/idset.h \
    reindexer/core/idsetcache.h \
    reindexer/core/indexdef.h \
    reindexer/core/indexopts.h \
    reindexer/core/item.h \
    reindexer/core/itemimpl.h \
    reindexer/core/joincache.h \
    reindexer/core/lrucache.h \
    reindexer/core/lsn.h \
    reindexer/core/namespacedef.h \
    reindexer/core/perfstatcounter.h \
    reindexer/core/querycache.h \
    reindexer/core/querystat.h \
    reindexer/core/rdxcontext.h \
    reindexer/core/reindexer.h \
    reindexer/core/reindexerimpl.h \
    reindexer/core/schema.h \
    reindexer/core/selectkeyresult.h \
    reindexer/core/sortingprioritiestable.h \
    reindexer/core/transaction.h \
    reindexer/core/transactionimpl.h \
    reindexer/core/txstats.h \
    reindexer/core/type_consts.h \
    reindexer/debug/allocdebug.h \
    reindexer/debug/backtrace.h \
    reindexer/debug/resolver.h \
    reindexer/debug/terminate_handler.h \
    reindexer/estl/atomic_unique_ptr.h \
    reindexer/estl/cbuf.h \
    reindexer/estl/chunk_buf.h \
    reindexer/estl/contexted_locks.h \
    reindexer/estl/cow.h \
    reindexer/estl/fast_hash_map.h \
    reindexer/estl/fast_hash_set.h \
    reindexer/estl/flat_str_map.h \
    reindexer/estl/h_vector.h \
    reindexer/estl/intrusive_ptr.h \
    reindexer/estl/mutex.h \
    reindexer/estl/packed_vector.h \
    reindexer/estl/shared_mutex.h \
    reindexer/estl/smart_lock.h \
    reindexer/estl/span.h \
    reindexer/estl/string_view.h \
    reindexer/estl/suffix_map.h \
    reindexer/estl/syncpool.h \
    reindexer/estl/tokenizer.h \
    reindexer/estl/trivial_reverse_iterator.h \
    reindexer/net/cproto/args.h \
    reindexer/net/cproto/clientconnection.h \
    reindexer/net/cproto/cproto.h \
    reindexer/net/cproto/dispatcher.h \
    reindexer/net/cproto/serverconnection.h \
    reindexer/net/ev/ev.h \
    reindexer/net/http/router.h \
    reindexer/net/http/serverconnection.h \
    reindexer/net/connection.h \
    reindexer/net/iserverconnection.h \
    reindexer/net/listener.h \
    reindexer/net/socket.h \
    reindexer/net/stat.h \
    reindexer/replicator/replicator.h \
    reindexer/replicator/updatesobserver.h \
    reindexer/replicator/walrecord.h \
    reindexer/replicator/walselecter.h \
    reindexer/replicator/waltracker.h \
    reindexer/tools/alloc_ext/je_malloc_extension.h \
    reindexer/tools/alloc_ext/tc_malloc_extension.h \
    reindexer/tools/customhash.h \
    reindexer/tools/customlocal.h \
    reindexer/tools/errors.h \
    reindexer/tools/filecontentwatcher.h \
    reindexer/tools/flagguard.h \
    reindexer/tools/fsops.h \
    reindexer/tools/json2kv.h \
    reindexer/tools/jsontools.h \
    reindexer/tools/logger.h \
    reindexer/tools/md5crypt.h \
    reindexer/tools/oscompat.h \
    reindexer/tools/semversion.h \
    reindexer/tools/serializer.h \
    reindexer/tools/ssize_t.h \
    reindexer/tools/string_regexp_functions.h \
    reindexer/tools/stringstools.h \
    reindexer/tools/terminalutils.h \
    reindexer/tools/timetools.h \
    reindexer/tools/varint.h \
    reindexer/vendor/atoi/atoi.h \
    reindexer/vendor/base64/base64.h \
    reindexer/vendor/cpp-btree/btree.h \
    reindexer/vendor/cpp-btree/btree_container.h \
    reindexer/vendor/cpp-btree/btree_map.h \
    reindexer/vendor/cpp-btree/btree_set.h \
    reindexer/vendor/cpp-btree/btree_test.h \
    reindexer/vendor/cpp-btree/safe_btree.h \
    reindexer/vendor/cpp-btree/safe_btree_map.h \
    reindexer/vendor/cpp-btree/safe_btree_set.h \
    reindexer/vendor/double-conversion/bignum-dtoa.h \
    reindexer/vendor/double-conversion/bignum.h \
    reindexer/vendor/double-conversion/cached-powers.h \
    reindexer/vendor/double-conversion/diy-fp.h \
    reindexer/vendor/double-conversion/double-conversion.h \
    reindexer/vendor/double-conversion/fast-dtoa.h \
    reindexer/vendor/double-conversion/fixed-dtoa.h \
    reindexer/vendor/double-conversion/ieee.h \
    reindexer/vendor/double-conversion/strtod.h \
    reindexer/vendor/double-conversion/utils.h \
    reindexer/vendor/gason/gason.h \
    reindexer/vendor/hash/md5.h \
    reindexer/vendor/hopscotch/hopscotch_hash.h \
    reindexer/vendor/hopscotch/hopscotch_map.h \
    reindexer/vendor/hopscotch/hopscotch_sc_map.h \
    reindexer/vendor/hopscotch/hopscotch_sc_set.h \
    reindexer/vendor/hopscotch/hopscotch_set.h \
    reindexer/vendor/itoa/itoa.h \
    reindexer/vendor/libbacktrace/backtrace.h \
    reindexer/vendor/libbacktrace/config.h \
    reindexer/vendor/libbacktrace/internal.h \
    reindexer/vendor/libdivsufsort/config.h \
    reindexer/vendor/libdivsufsort/divsufsort.h \
    reindexer/vendor/libdivsufsort/divsufsort_private.h \
    reindexer/vendor/libstemmer/include/libstemmer.h \
    reindexer/vendor/libstemmer/libstemmer/modules_utf8.h \
    reindexer/vendor/libstemmer/runtime/api.h \
    reindexer/vendor/libstemmer/runtime/header.h \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_danish.h \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_dutch.h \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_english.h \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_finnish.h \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_french.h \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_german.h \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_hungarian.h \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_italian.h \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_norwegian.h \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_porter.h \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_portuguese.h \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_romanian.h \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_russian.h \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_spanish.h \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_swedish.h \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_turkish.h \
    reindexer/vendor/mpark/variant.h \
    reindexer/vendor/msgpack/msgpack/predef/architecture/x86/32.h \
    reindexer/vendor/msgpack/msgpack/predef/architecture/x86/64.h \
    reindexer/vendor/msgpack/msgpack/predef/architecture/alpha.h \
    reindexer/vendor/msgpack/msgpack/predef/architecture/arm.h \
    reindexer/vendor/msgpack/msgpack/predef/architecture/blackfin.h \
    reindexer/vendor/msgpack/msgpack/predef/architecture/convex.h \
    reindexer/vendor/msgpack/msgpack/predef/architecture/ia64.h \
    reindexer/vendor/msgpack/msgpack/predef/architecture/m68k.h \
    reindexer/vendor/msgpack/msgpack/predef/architecture/mips.h \
    reindexer/vendor/msgpack/msgpack/predef/architecture/parisc.h \
    reindexer/vendor/msgpack/msgpack/predef/architecture/ppc.h \
    reindexer/vendor/msgpack/msgpack/predef/architecture/ptx.h \
    reindexer/vendor/msgpack/msgpack/predef/architecture/pyramid.h \
    reindexer/vendor/msgpack/msgpack/predef/architecture/rs6k.h \
    reindexer/vendor/msgpack/msgpack/predef/architecture/sparc.h \
    reindexer/vendor/msgpack/msgpack/predef/architecture/superh.h \
    reindexer/vendor/msgpack/msgpack/predef/architecture/sys370.h \
    reindexer/vendor/msgpack/msgpack/predef/architecture/sys390.h \
    reindexer/vendor/msgpack/msgpack/predef/architecture/x86.h \
    reindexer/vendor/msgpack/msgpack/predef/architecture/z.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/borland.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/clang.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/comeau.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/compaq.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/diab.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/digitalmars.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/dignus.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/edg.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/ekopath.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/gcc.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/gcc_xml.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/greenhills.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/hp_acc.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/iar.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/ibm.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/intel.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/kai.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/llvm.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/metaware.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/metrowerks.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/microtec.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/mpw.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/nvcc.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/palm.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/pgi.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/sgi_mipspro.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/sunpro.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/tendra.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/visualc.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler/watcom.h \
    reindexer/vendor/msgpack/msgpack/predef/detail/_cassert.h \
    reindexer/vendor/msgpack/msgpack/predef/detail/_exception.h \
    reindexer/vendor/msgpack/msgpack/predef/detail/comp_detected.h \
    reindexer/vendor/msgpack/msgpack/predef/detail/endian_compat.h \
    reindexer/vendor/msgpack/msgpack/predef/detail/os_detected.h \
    reindexer/vendor/msgpack/msgpack/predef/detail/platform_detected.h \
    reindexer/vendor/msgpack/msgpack/predef/detail/test.h \
    reindexer/vendor/msgpack/msgpack/predef/detail/test_def.h \
    reindexer/vendor/msgpack/msgpack/predef/hardware/simd/arm/versions.h \
    reindexer/vendor/msgpack/msgpack/predef/hardware/simd/ppc/versions.h \
    reindexer/vendor/msgpack/msgpack/predef/hardware/simd/x86/versions.h \
    reindexer/vendor/msgpack/msgpack/predef/hardware/simd/x86_amd/versions.h \
    reindexer/vendor/msgpack/msgpack/predef/hardware/simd/arm.h \
    reindexer/vendor/msgpack/msgpack/predef/hardware/simd/ppc.h \
    reindexer/vendor/msgpack/msgpack/predef/hardware/simd/x86.h \
    reindexer/vendor/msgpack/msgpack/predef/hardware/simd/x86_amd.h \
    reindexer/vendor/msgpack/msgpack/predef/hardware/simd.h \
    reindexer/vendor/msgpack/msgpack/predef/language/cuda.h \
    reindexer/vendor/msgpack/msgpack/predef/language/objc.h \
    reindexer/vendor/msgpack/msgpack/predef/language/stdc.h \
    reindexer/vendor/msgpack/msgpack/predef/language/stdcpp.h \
    reindexer/vendor/msgpack/msgpack/predef/library/c/_prefix.h \
    reindexer/vendor/msgpack/msgpack/predef/library/c/cloudabi.h \
    reindexer/vendor/msgpack/msgpack/predef/library/c/gnu.h \
    reindexer/vendor/msgpack/msgpack/predef/library/c/uc.h \
    reindexer/vendor/msgpack/msgpack/predef/library/c/vms.h \
    reindexer/vendor/msgpack/msgpack/predef/library/c/zos.h \
    reindexer/vendor/msgpack/msgpack/predef/library/std/_prefix.h \
    reindexer/vendor/msgpack/msgpack/predef/library/std/cxx.h \
    reindexer/vendor/msgpack/msgpack/predef/library/std/dinkumware.h \
    reindexer/vendor/msgpack/msgpack/predef/library/std/libcomo.h \
    reindexer/vendor/msgpack/msgpack/predef/library/std/modena.h \
    reindexer/vendor/msgpack/msgpack/predef/library/std/msl.h \
    reindexer/vendor/msgpack/msgpack/predef/library/std/roguewave.h \
    reindexer/vendor/msgpack/msgpack/predef/library/std/sgi.h \
    reindexer/vendor/msgpack/msgpack/predef/library/std/stdcpp3.h \
    reindexer/vendor/msgpack/msgpack/predef/library/std/stlport.h \
    reindexer/vendor/msgpack/msgpack/predef/library/std/vacpp.h \
    reindexer/vendor/msgpack/msgpack/predef/library/c.h \
    reindexer/vendor/msgpack/msgpack/predef/library/std.h \
    reindexer/vendor/msgpack/msgpack/predef/os/bsd/bsdi.h \
    reindexer/vendor/msgpack/msgpack/predef/os/bsd/dragonfly.h \
    reindexer/vendor/msgpack/msgpack/predef/os/bsd/free.h \
    reindexer/vendor/msgpack/msgpack/predef/os/bsd/net.h \
    reindexer/vendor/msgpack/msgpack/predef/os/bsd/open.h \
    reindexer/vendor/msgpack/msgpack/predef/os/aix.h \
    reindexer/vendor/msgpack/msgpack/predef/os/amigaos.h \
    reindexer/vendor/msgpack/msgpack/predef/os/android.h \
    reindexer/vendor/msgpack/msgpack/predef/os/beos.h \
    reindexer/vendor/msgpack/msgpack/predef/os/bsd.h \
    reindexer/vendor/msgpack/msgpack/predef/os/cygwin.h \
    reindexer/vendor/msgpack/msgpack/predef/os/haiku.h \
    reindexer/vendor/msgpack/msgpack/predef/os/hpux.h \
    reindexer/vendor/msgpack/msgpack/predef/os/ios.h \
    reindexer/vendor/msgpack/msgpack/predef/os/irix.h \
    reindexer/vendor/msgpack/msgpack/predef/os/linux.h \
    reindexer/vendor/msgpack/msgpack/predef/os/macos.h \
    reindexer/vendor/msgpack/msgpack/predef/os/os400.h \
    reindexer/vendor/msgpack/msgpack/predef/os/qnxnto.h \
    reindexer/vendor/msgpack/msgpack/predef/os/solaris.h \
    reindexer/vendor/msgpack/msgpack/predef/os/unix.h \
    reindexer/vendor/msgpack/msgpack/predef/os/vms.h \
    reindexer/vendor/msgpack/msgpack/predef/os/windows.h \
    reindexer/vendor/msgpack/msgpack/predef/other/endian.h \
    reindexer/vendor/msgpack/msgpack/predef/other/workaround.h \
    reindexer/vendor/msgpack/msgpack/predef/platform/cloudabi.h \
    reindexer/vendor/msgpack/msgpack/predef/platform/ios.h \
    reindexer/vendor/msgpack/msgpack/predef/platform/mingw.h \
    reindexer/vendor/msgpack/msgpack/predef/platform/mingw32.h \
    reindexer/vendor/msgpack/msgpack/predef/platform/mingw64.h \
    reindexer/vendor/msgpack/msgpack/predef/platform/windows_desktop.h \
    reindexer/vendor/msgpack/msgpack/predef/platform/windows_phone.h \
    reindexer/vendor/msgpack/msgpack/predef/platform/windows_runtime.h \
    reindexer/vendor/msgpack/msgpack/predef/platform/windows_server.h \
    reindexer/vendor/msgpack/msgpack/predef/platform/windows_store.h \
    reindexer/vendor/msgpack/msgpack/predef/platform/windows_system.h \
    reindexer/vendor/msgpack/msgpack/predef/platform/windows_uwp.h \
    reindexer/vendor/msgpack/msgpack/predef/architecture.h \
    reindexer/vendor/msgpack/msgpack/predef/compiler.h \
    reindexer/vendor/msgpack/msgpack/predef/hardware.h \
    reindexer/vendor/msgpack/msgpack/predef/language.h \
    reindexer/vendor/msgpack/msgpack/predef/library.h \
    reindexer/vendor/msgpack/msgpack/predef/make.h \
    reindexer/vendor/msgpack/msgpack/predef/os.h \
    reindexer/vendor/msgpack/msgpack/predef/other.h \
    reindexer/vendor/msgpack/msgpack/predef/platform.h \
    reindexer/vendor/msgpack/msgpack/predef/version.h \
    reindexer/vendor/msgpack/msgpack/predef/version_number.h \
    reindexer/vendor/msgpack/msgpack/fbuffer.h \
    reindexer/vendor/msgpack/msgpack/object.h \
    reindexer/vendor/msgpack/msgpack/pack.h \
    reindexer/vendor/msgpack/msgpack/pack_define.h \
    reindexer/vendor/msgpack/msgpack/pack_template.h \
    reindexer/vendor/msgpack/msgpack/sbuffer.h \
    reindexer/vendor/msgpack/msgpack/sysdep.h \
    reindexer/vendor/msgpack/msgpack/timestamp.h \
    reindexer/vendor/msgpack/msgpack/unpack.h \
    reindexer/vendor/msgpack/msgpack/unpack_define.h \
    reindexer/vendor/msgpack/msgpack/unpack_template.h \
    reindexer/vendor/msgpack/msgpack/util.h \
    reindexer/vendor/msgpack/msgpack/version.h \
    reindexer/vendor/msgpack/msgpack/version_master.h \
    reindexer/vendor/msgpack/msgpack/vrefbuffer.h \
    reindexer/vendor/msgpack/msgpack/zbuffer.h \
    reindexer/vendor/msgpack/msgpack/zone.h \
    reindexer/vendor/msgpack/msgpack.h \
    reindexer/vendor/msgpack/msgpackparser.h \
    reindexer/vendor/murmurhash/MurmurHash3.h \
    reindexer/vendor/picohttpparser/picohttpparser.h \
    reindexer/vendor/prometheus/detail/ckms_quantiles.h \
    reindexer/vendor/prometheus/detail/counter_builder.h \
    reindexer/vendor/prometheus/detail/future_std.h \
    reindexer/vendor/prometheus/detail/gauge_builder.h \
    reindexer/vendor/prometheus/detail/histogram_builder.h \
    reindexer/vendor/prometheus/detail/summary_builder.h \
    reindexer/vendor/prometheus/detail/time_window_quantiles.h \
    reindexer/vendor/prometheus/detail/utils.h \
    reindexer/vendor/prometheus/impl/detail/hash.h \
    reindexer/vendor/prometheus/check_names.h \
    reindexer/vendor/prometheus/client_metric.h \
    reindexer/vendor/prometheus/collectable.h \
    reindexer/vendor/prometheus/counter.h \
    reindexer/vendor/prometheus/family.h \
    reindexer/vendor/prometheus/gauge.h \
    reindexer/vendor/prometheus/histogram.h \
    reindexer/vendor/prometheus/metric_family.h \
    reindexer/vendor/prometheus/metric_type.h \
    reindexer/vendor/prometheus/registry.h \
    reindexer/vendor/prometheus/serializer.h \
    reindexer/vendor/prometheus/summary.h \
    reindexer/vendor/prometheus/text_serializer.h \
    reindexer/vendor/sparse-map/sparse_growth_policy.h \
    reindexer/vendor/sparse-map/sparse_hash.h \
    reindexer/vendor/sparse-map/sparse_map.h \
    reindexer/vendor/sparse-map/sparse_set.h \
    reindexer/vendor/spdlog/details/async_log_helper.h \
    reindexer/vendor/spdlog/details/async_logger_impl.h \
    reindexer/vendor/spdlog/details/file_helper.h \
    reindexer/vendor/spdlog/details/log_msg.h \
    reindexer/vendor/spdlog/details/logger_impl.h \
    reindexer/vendor/spdlog/details/mpmc_bounded_q.h \
    reindexer/vendor/spdlog/details/null_mutex.h \
    reindexer/vendor/spdlog/details/os.h \
    reindexer/vendor/spdlog/details/pattern_formatter_impl.h \
    reindexer/vendor/spdlog/details/registry.h \
    reindexer/vendor/spdlog/details/spdlog_impl.h \
    reindexer/vendor/spdlog/fmt/bundled/format.h \
    reindexer/vendor/spdlog/fmt/bundled/ostream.h \
    reindexer/vendor/spdlog/fmt/bundled/posix.h \
    reindexer/vendor/spdlog/fmt/bundled/printf.h \
    reindexer/vendor/spdlog/fmt/bundled/time.h \
    reindexer/vendor/spdlog/fmt/fmt.h \
    reindexer/vendor/spdlog/fmt/ostr.h \
    reindexer/vendor/spdlog/sinks/android_sink.h \
    reindexer/vendor/spdlog/sinks/ansicolor_sink.h \
    reindexer/vendor/spdlog/sinks/base_sink.h \
    reindexer/vendor/spdlog/sinks/dist_sink.h \
    reindexer/vendor/spdlog/sinks/file_sinks.h \
    reindexer/vendor/spdlog/sinks/msvc_sink.h \
    reindexer/vendor/spdlog/sinks/null_sink.h \
    reindexer/vendor/spdlog/sinks/ostream_sink.h \
    reindexer/vendor/spdlog/sinks/sink.h \
    reindexer/vendor/spdlog/sinks/stdout_sinks.h \
    reindexer/vendor/spdlog/sinks/syslog_sink.h \
    reindexer/vendor/spdlog/sinks/wincolor_sink.h \
    reindexer/vendor/spdlog/sinks/windebug_sink.h \
    reindexer/vendor/spdlog/async_logger.h \
    reindexer/vendor/spdlog/common.h \
    reindexer/vendor/spdlog/formatter.h \
    reindexer/vendor/spdlog/logger.h \
    reindexer/vendor/spdlog/spdlog.h \
    reindexer/vendor/spdlog/tweakme.h \
    reindexer/vendor/time/fast_time.h \
    reindexer/vendor/urlparser/urlparser.h \
    reindexer/vendor/utf8cpp/utf8/checked.h \
    reindexer/vendor/utf8cpp/utf8/core.h \
    reindexer/vendor/utf8cpp/utf8/unchecked.h \
    reindexer/vendor/utf8cpp/utf8.h \
    reindexer/vendor/wcwidth/wcwidth.h \
    reindexer/vendor/yaml/yaml.h
#

# REINDEXER Sources
REINDEXER_CC = \
    reindexer/client/item.cc \
    reindexer/client/itemimpl.cc \
    reindexer/client/namespace.cc \
    reindexer/client/queryresults.cc \
    reindexer/client/reindexer.cc \
    reindexer/client/resultserializer.cc \
    reindexer/client/rpcclient.cc \
    reindexer/client/rpcclientmock.cc \
    reindexer/client/transaction.cc \
    reindexer/core/cbinding/reindexer_c.cc \
    reindexer/core/cbinding/resultserializer.cc \
    reindexer/core/cjson/baseencoder.cc \
    reindexer/core/cjson/cjsonbuilder.cc \
    reindexer/core/cjson/cjsondecoder.cc \
    reindexer/core/cjson/cjsonmodifier.cc \
    reindexer/core/cjson/cjsontools.cc \
    reindexer/core/cjson/jschemachecker.cc \
    reindexer/core/cjson/jsonbuilder.cc \
    reindexer/core/cjson/jsondecoder.cc \
    reindexer/core/cjson/msgpackbuilder.cc \
    reindexer/core/cjson/msgpackdecoder.cc \
    reindexer/core/cjson/readme.md \
    reindexer/core/cjson/tagslengths.cc \
    reindexer/core/ft/config/baseftconfig.cc \
    reindexer/core/ft/config/ftfastconfig.cc \
    reindexer/core/ft/config/ftfuzzyconfig.cc \
    reindexer/core/ft/filters/kblayout.cc \
    reindexer/core/ft/filters/synonyms.cc \
    reindexer/core/ft/filters/translit.cc \
    reindexer/core/ft/ft_fast/dataholder.cc \
    reindexer/core/ft/ft_fast/dataprocessor.cc \
    reindexer/core/ft/ft_fast/selecter.cc \
    reindexer/core/ft/ft_fuzzy/dataholder/basebuildedholder.cc \
    reindexer/core/ft/ft_fuzzy/dataholder/smardeque.cc \
    reindexer/core/ft/ft_fuzzy/dumper/fulltextdumper.cc \
    reindexer/core/ft/ft_fuzzy/merger/basemerger.cc \
    reindexer/core/ft/ft_fuzzy/prefilter/prefilter.cc \
    reindexer/core/ft/ft_fuzzy/advacedpackedvec.cc \
    reindexer/core/ft/ft_fuzzy/baseseacher.cc \
    reindexer/core/ft/ft_fuzzy/searchengine.cc \
    reindexer/core/ft/stopwords/stop_en.cc \
    reindexer/core/ft/stopwords/stop_ru.cc \
    reindexer/core/ft/areaholder.cc \
    reindexer/core/ft/ftdsl.cc \
    reindexer/core/ft/idrelset.cc \
    reindexer/core/ft/numtotext.cc \
    reindexer/core/ft/typos.cc \
    reindexer/core/index/indextext/fastindextext.cc \
    reindexer/core/index/indextext/fuzzyindextext.cc \
    reindexer/core/index/indextext/indextext.cc \
    reindexer/core/index/index.cc \
    reindexer/core/index/indexordered.cc \
    reindexer/core/index/indexstore.cc \
    reindexer/core/index/indexunordered.cc \
    reindexer/core/index/ttlindex.cc \
    reindexer/core/keyvalue/variant.cc \
    reindexer/core/namespace/namespace.cc \
    reindexer/core/namespace/namespaceimpl.cc \
    reindexer/core/namespace/namespacestat.cc \
    reindexer/core/nsselecter/aggregator.cc \
    reindexer/core/nsselecter/explaincalc.cc \
    reindexer/core/nsselecter/itemcomparator.cc \
    reindexer/core/nsselecter/joinedselector.cc \
    reindexer/core/nsselecter/nsselecter.cc \
    reindexer/core/nsselecter/querypreprocessor.cc \
    reindexer/core/nsselecter/selectiterator.cc \
    reindexer/core/nsselecter/selectiteratorcontainer.cc \
    reindexer/core/nsselecter/sortexpression.cc \
    reindexer/core/nsselecter/sortingcontext.cc \
    reindexer/core/payload/payloadfieldtype.cc \
    reindexer/core/payload/payloadfieldvalue.cc \
    reindexer/core/payload/payloadiface.cc \
    reindexer/core/payload/payloadtype.cc \
    reindexer/core/payload/payloadvalue.cc \
    reindexer/core/payload/readme.md \
    reindexer/core/query/dsl/dslencoder.cc \
    reindexer/core/query/dsl/dslparser.cc \
    reindexer/core/query/sql/sqlencoder.cc \
    reindexer/core/query/sql/sqlparser.cc \
    reindexer/core/query/sql/sqlsuggester.cc \
    reindexer/core/query/expressionevaluator.cc \
    reindexer/core/query/query.cc \
    reindexer/core/query/queryentry.cc \
    reindexer/core/queryresults/aggregationresult.cc \
    reindexer/core/queryresults/joinresults.cc \
    reindexer/core/queryresults/queryresults.cc \
    reindexer/core/queryresults/tableviewbuilder.cc \
    reindexer/core/selectfunc/ctx/ftctx.cc \
    reindexer/core/selectfunc/functions/highlight.cc \
    reindexer/core/selectfunc/functions/snippet.cc \
    reindexer/core/selectfunc/functionexecutor.cc \
    reindexer/core/selectfunc/nsselectfuncinterface.cc \
    reindexer/core/selectfunc/selectfunc.cc \
    reindexer/core/selectfunc/selectfuncparser.cc \
    reindexer/core/storage/basestorage.cc \
    reindexer/core/storage/leveldbstorage.cc \
    reindexer/core/storage/rocksdbstorage.cc \
    reindexer/core/storage/storagefactory.cc \
    reindexer/core/activity_context.cc \
    reindexer/core/comparator.cc \
    reindexer/core/compositearraycomparator.cc \
    reindexer/core/dbconfig.cc \
    reindexer/core/expressiontree.cc \
    reindexer/core/expressiontree.md \
    reindexer/core/iclientsstats.cc \
    reindexer/core/idset.cc \
    reindexer/core/indexdef.cc \
    reindexer/core/indexopts.cc \
    reindexer/core/item.cc \
    reindexer/core/itemimpl.cc \
    reindexer/core/lrucache.cc \
    reindexer/core/lsn.cc \
    reindexer/core/namespacedef.cc \
    reindexer/core/perfstatcounter.cc \
    reindexer/core/querystat.cc \
    reindexer/core/rdxcontext.cc \
    reindexer/core/reindexer.cc \
    reindexer/core/reindexerimpl.cc \
    reindexer/core/schema.cc \
    reindexer/core/sortingprioritiestable.cc \
    reindexer/core/transaction.cc \
    reindexer/core/transactionimpl.cc \
    reindexer/debug/allocdebug.cc \
    reindexer/debug/backtrace.cc \
    reindexer/debug/resolver.cc \
    reindexer/debug/terminate_handler.cpp \
    reindexer/estl/mutex.cc \
    reindexer/estl/tokenizer.cc \
    reindexer/net/cproto/args.cc \
    reindexer/net/cproto/clientconnection.cc \
    reindexer/net/cproto/cproto.cc \
    reindexer/net/cproto/dispatcher.cc \
    reindexer/net/cproto/serverconnection.cc \
    reindexer/net/ev/ev.cc \
    reindexer/net/http/router.cc \
    reindexer/net/http/serverconnection.cc \
    reindexer/net/connection.cc \
    reindexer/net/listener.cc \
    reindexer/net/socket.cc \
    reindexer/net/stat.cc \
    reindexer/replicator/replication.conf \
    reindexer/replicator/replicator.cc \
    reindexer/replicator/updatesobserver.cc \
    reindexer/replicator/walrecord.cc \
    reindexer/replicator/walselecter.cc \
    reindexer/replicator/waltracker.cc \
    reindexer/tools/alloc_ext/je_malloc_extension.cc \
    reindexer/tools/alloc_ext/tc_malloc_extension.cc \
    reindexer/tools/alloc_ext/tc_malloc_hook.cc \
    reindexer/tools/customhash.cc \
    reindexer/tools/customlocal.cc \
    reindexer/tools/errors.cc \
    reindexer/tools/fsops.cc \
    reindexer/tools/json2kv.cc \
    reindexer/tools/jsontools.cc \
    reindexer/tools/logger.cc \
    reindexer/tools/md5crypt.cc \
    reindexer/tools/serializer.cc \
    reindexer/tools/string_regexp_functions.cc \
    reindexer/tools/stringstools.cc \
    reindexer/tools/terminalutils.cc \
    reindexer/tools/timetools.cc \
    reindexer/vendor/args/args.hpp \
    reindexer/vendor/base64/base64.c \
    reindexer/vendor/cpp-btree/COPYING \
    reindexer/vendor/cpp-btree/README \
    reindexer/vendor/double-conversion/bignum-dtoa.cc \
    reindexer/vendor/double-conversion/bignum.cc \
    reindexer/vendor/double-conversion/cached-powers.cc \
    reindexer/vendor/double-conversion/diy-fp.cc \
    reindexer/vendor/double-conversion/double-conversion.cc \
    reindexer/vendor/double-conversion/fast-dtoa.cc \
    reindexer/vendor/double-conversion/fixed-dtoa.cc \
    reindexer/vendor/double-conversion/strtod.cc \
    reindexer/vendor/gason/gason.cc \
    reindexer/vendor/hash/md5.cc \
    reindexer/vendor/itoa/itoa.cc \
    reindexer/vendor/libbacktrace/dwarf.c \
    reindexer/vendor/libbacktrace/elf.c \
    reindexer/vendor/libbacktrace/fileline.c \
    reindexer/vendor/libbacktrace/mmap.c \
    reindexer/vendor/libbacktrace/mmapio.c \
    reindexer/vendor/libbacktrace/posix.c \
    reindexer/vendor/libbacktrace/sort.c \
    reindexer/vendor/libbacktrace/state.c \
    reindexer/vendor/libdivsufsort/divsufsort.c \
    reindexer/vendor/libdivsufsort/LICENSE \
    reindexer/vendor/libdivsufsort/README.md \
    reindexer/vendor/libdivsufsort/sssort.c \
    reindexer/vendor/libdivsufsort/trsort.c \
    reindexer/vendor/libdivsufsort/utils.c \
    reindexer/vendor/libstemmer/libstemmer/libstemmer_utf8.c \
    reindexer/vendor/libstemmer/libstemmer/modules_utf8.txt \
    reindexer/vendor/libstemmer/runtime/api.c \
    reindexer/vendor/libstemmer/runtime/utilities.c \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_danish.c \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_dutch.c \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_english.c \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_finnish.c \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_french.c \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_german.c \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_hungarian.c \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_italian.c \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_norwegian.c \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_porter.c \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_portuguese.c \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_romanian.c \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_russian.c \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_spanish.c \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_swedish.c \
    reindexer/vendor/libstemmer/src_c/stem_UTF_8_turkish.c \
    reindexer/vendor/libstemmer/README \
    reindexer/vendor/msgpack/msgpack.cc \
    reindexer/vendor/msgpack/msgpackparser.cc \
    reindexer/vendor/msgpack/objectc.c \
    reindexer/vendor/msgpack/unpack.c \
    reindexer/vendor/msgpack/version.c \
    reindexer/vendor/msgpack/vrefbuffer.c \
    reindexer/vendor/msgpack/zone.c \
    reindexer/vendor/murmurhash/MurmurHash3.cc \
    reindexer/vendor/picohttpparser/picohttpparser.c \
    reindexer/vendor/prometheus/impl/detail/ckms_quantiles.cc \
    reindexer/vendor/prometheus/impl/detail/counter_builder.cc \
    reindexer/vendor/prometheus/impl/detail/gauge_builder.cc \
    reindexer/vendor/prometheus/impl/detail/histogram_builder.cc \
    reindexer/vendor/prometheus/impl/detail/summary_builder.cc \
    reindexer/vendor/prometheus/impl/detail/time_window_quantiles.cc \
    reindexer/vendor/prometheus/impl/detail/utils.cc \
    reindexer/vendor/prometheus/impl/check_names.cc \
    reindexer/vendor/prometheus/impl/counter.cc \
    reindexer/vendor/prometheus/impl/gauge.cc \
    reindexer/vendor/prometheus/impl/histogram.cc \
    reindexer/vendor/prometheus/impl/registry.cc \
    reindexer/vendor/prometheus/impl/serializer.cc \
    reindexer/vendor/prometheus/impl/summary.cc \
    reindexer/vendor/prometheus/impl/text_serializer.cc \
    reindexer/vendor/sort/pdqsort.hpp \
    reindexer/vendor/spdlog/fmt/bundled/format.cc \
    reindexer/vendor/spdlog/fmt/bundled/LICENSE.rst \
    reindexer/vendor/spdlog/fmt/bundled/ostream.cc \
    reindexer/vendor/spdlog/fmt/bundled/posix.cc \
    reindexer/vendor/spdlog/fmt/bundled/printf.cc \
    reindexer/vendor/time/gmtime.c \
    reindexer/vendor/wcwidth/wcwidth.cc \
    reindexer/vendor/yaml/yaml.cc
#

# pocketcoin core #
POCKETCOIN_CORE_H = \
    addrdb.h \
    addrman.h \
    base58.h \
    bech32.h \
    bloom.h \
    blockencodings.h \
    blockfilter.h \
    chain.h \
    chainparams.h \
    chainparamsbase.h \
    chainparamsseeds.h \
    checkpoints.h \
    checkqueue.h \
    clientversion.h \
    coins.h \
    compat.h \
    compat/byteswap.h \
    compat/endian.h \
    compat/sanity.h \
    compressor.h \
    consensus/consensus.h \
    consensus/tx_verify.h \
    core_io.h \
    core_memusage.h \
    cuckoocache.h \
    fs.h \
    httprpc.h \
    httpserver.h \
    index/base.h \
    index/txindex.h \
    indirectmap.h \
    init.h \
    interfaces/handler.h \
    interfaces/node.h \
    interfaces/wallet.h \
    key.h \
    key_io.h \
    keystore.h \
    dbwrapper.h \
    ldb/ldb.h \
    limitedmap.h \
    logging.h \
    memusage.h \
    merkleblock.h \
    miner.h \
    net.h \
    net_processing.h \
    netaddress.h \
    netbase.h \
    netmessagemaker.h \
    noui.h \
    outputtype.h \
    policy/feerate.h \
    policy/fees.h \
    policy/policy.h \
    policy/rbf.h \
    pos.h \
    pow.h \
    protocol.h \
    random.h \
    reverse_iterator.h \
    reverselock.h \
    rpc/blockchain.h \
    rpc/client.h \
    rpc/mining.h \
    rpc/protocol.h \
    rpc/server.h \
    rpc/rawtransaction.h \
    rpc/register.h \
    rpc/util.h \
    rpc/pocketrpc.h \
    scheduler.h \
    script/descriptor.h \
    script/ismine.h \
    script/sigcache.h \
    script/sign.h \
    script/standard.h \
    shutdown.h \
    staker.h \
    streams.h \
    support/allocators/secure.h \
    support/allocators/zeroafterfree.h \
    support/cleanse.h \
    support/events.h \
    support/lockedpool.h \
    sync.h \
    threadsafety.h \
    threadinterrupt.h \
    timedata.h \
    torcontrol.h \
    txdb.h \
    txmempool.h \
    ui_interface.h \
    undo.h \
    util.h \
    utilmemory.h \
    utilmoneystr.h \
    utiltime.h \
    validation.h \
    validationinterface.h \
    versionbits.h \
    walletinitinterface.h \
    wallet/coincontrol.h \
    wallet/crypter.h \
    wallet/db.h \
    wallet/feebumper.h \
    wallet/fees.h \
    wallet/rpcwallet.h \
    wallet/wallet.h \
    wallet/walletdb.h \
    wallet/walletutil.h \
    wallet/coinselection.h \
    warnings.h \
    zmq/zmqabstractnotifier.h \
    zmq/zmqconfig.h\
    zmq/zmqnotificationinterface.h \
    zmq/zmqpublishnotifier.h \
    zmq/zmqrpc.h \
    pocketdb/pocketdb.h \
    antibot/antibot.h \
    index/addrindex.h \
    websocket/ws.h \
    primitives/rtransaction.cpp \
    primitives/rtransaction.h \
    html.cpp \
    html.h \
    pocketdb/pocketnet.cpp \
    pocketdb/pocketnet.h

obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh "$(abs_top_builddir)/src/obj/build.h" \
	  "$(abs_top_srcdir)"
libpocketcoin_util_a-clientversion.$(OBJEXT): obj/build.h

# reindexer library
libreindexer_a_CPPFLAGS = $(REINDEXER_INCLUDES) -DREINDEX_WITH_LEVELDB
libreindexer_a_CXXFLAGS = $(REINDEXER_INCLUDES)
libreindexer_a_SOURCES = \
    $(REINDEXER_H) \
    $(REINDEXER_CC)
#

# server: shared between pocketcoind and pocketcoin-qt
libpocketcoin_server_a_CPPFLAGS = $(AM_CPPFLAGS) $(POCKETCOIN_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(EVENT_CFLAGS) $(EVENT_PTHREADS_CFLAGS)
libpocketcoin_server_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libpocketcoin_server_a_SOURCES = \
    addrdb.cpp \
    addrman.cpp \
    bloom.cpp \
    blockencodings.cpp \
    blockfilter.cpp \
    chain.cpp \
    checkpoints.cpp \
    consensus/tx_verify.cpp \
    httprpc.cpp \
    httpserver.cpp \
    index/base.cpp \
    index/txindex.cpp \
    interfaces/handler.cpp \
    interfaces/node.cpp \
    init.cpp  \
    ldb/ldb.cpp \
    dbwrapper.cpp \
    merkleblock.cpp \
    miner.cpp \
    net.cpp \
    net_processing.cpp \
    noui.cpp \
    outputtype.cpp \
    policy/fees.cpp \
    policy/policy.cpp \
    policy/rbf.cpp \
    pos.cpp \
    pow.cpp \
    rest.cpp \
    rpc/blockchain.cpp \
    rpc/mining.cpp \
    rpc/misc.cpp \
    rpc/net.cpp \
    rpc/rawtransaction.cpp \
    rpc/server.cpp \
    rpc/util.cpp \
    rpc/pocketrpc.cpp \
    script/sigcache.cpp \
    shutdown.cpp \
    staker.cpp \
    timedata.cpp \
    torcontrol.cpp \
    txdb.cpp \
    txmempool.cpp \
    ui_interface.cpp \
    validation.cpp \
    validationinterface.cpp \
    versionbits.cpp \
    pocketdb/pocketdb.cpp \
    antibot/antibot.cpp \
    index/addrindex.cpp \
    websocket/ws.cpp \
    $(REINDEXER_H) \
    $(REINDEXER_CC) \
    $(POCKETCOIN_CORE_H)

if !ENABLE_WALLET
libpocketcoin_server_a_SOURCES += dummywallet.cpp
endif

if ENABLE_ZMQ
libpocketcoin_zmq_a_CPPFLAGS = $(POCKETCOIN_INCLUDES) $(ZMQ_CFLAGS)
libpocketcoin_zmq_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libpocketcoin_zmq_a_SOURCES = \
  zmq/zmqabstractnotifier.cpp \
  zmq/zmqnotificationinterface.cpp \
  zmq/zmqpublishnotifier.cpp \
  zmq/zmqrpc.cpp
endif


# wallet: shared between pocketcoind and pocketcoin-qt, but only linked
# when wallet enabled
libpocketcoin_wallet_a_CPPFLAGS = $(AM_CPPFLAGS) $(POCKETCOIN_INCLUDES)
libpocketcoin_wallet_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libpocketcoin_wallet_a_SOURCES = \
    interfaces/wallet.cpp \
    wallet/coincontrol.cpp \
    wallet/crypter.cpp \
    wallet/db.cpp \
    wallet/feebumper.cpp \
    wallet/fees.cpp \
    wallet/init.cpp \
    wallet/rpcdump.cpp \
    wallet/rpcwallet.cpp \
    wallet/wallet.cpp \
    wallet/walletdb.cpp \
    wallet/walletutil.cpp \
    wallet/coinselection.cpp \
    $(REINDEXER_H) \
    $(POCKETCOIN_CORE_H)

# crypto primitives library
crypto_libpocketcoin_crypto_base_a_CPPFLAGS = $(AM_CPPFLAGS)
crypto_libpocketcoin_crypto_base_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libpocketcoin_crypto_base_a_SOURCES = \
  crypto/aes.cpp \
  crypto/aes.h \
  crypto/chacha20.h \
  crypto/chacha20.cpp \
  crypto/common.h \
  crypto/hmac_sha256.cpp \
  crypto/hmac_sha256.h \
  crypto/hmac_sha512.cpp \
  crypto/hmac_sha512.h \
  crypto/ripemd160.cpp \
  crypto/ripemd160.h \
  crypto/sha1.cpp \
  crypto/sha1.h \
  crypto/sha256.cpp \
  crypto/sha256.h \
  crypto/sha512.cpp \
  crypto/sha512.h

if USE_ASM
crypto_libpocketcoin_crypto_base_a_SOURCES += crypto/sha256_sse4.cpp
endif

crypto_libpocketcoin_crypto_sse41_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libpocketcoin_crypto_sse41_a_CPPFLAGS = $(AM_CPPFLAGS)
crypto_libpocketcoin_crypto_sse41_a_CXXFLAGS += $(SSE41_CXXFLAGS)
crypto_libpocketcoin_crypto_sse41_a_CPPFLAGS += -DENABLE_SSE41
crypto_libpocketcoin_crypto_sse41_a_SOURCES = crypto/sha256_sse41.cpp

crypto_libpocketcoin_crypto_avx2_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libpocketcoin_crypto_avx2_a_CPPFLAGS = $(AM_CPPFLAGS)
crypto_libpocketcoin_crypto_avx2_a_CXXFLAGS += $(AVX2_CXXFLAGS)
crypto_libpocketcoin_crypto_avx2_a_CPPFLAGS += -DENABLE_AVX2
crypto_libpocketcoin_crypto_avx2_a_SOURCES = crypto/sha256_avx2.cpp

crypto_libpocketcoin_crypto_shani_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libpocketcoin_crypto_shani_a_CPPFLAGS = $(AM_CPPFLAGS)
crypto_libpocketcoin_crypto_shani_a_CXXFLAGS += $(SHANI_CXXFLAGS)
crypto_libpocketcoin_crypto_shani_a_CPPFLAGS += -DENABLE_SHANI
crypto_libpocketcoin_crypto_shani_a_SOURCES = crypto/sha256_shani.cpp

# consensus: shared between all executables that validate any consensus rules.
libpocketcoin_consensus_a_CPPFLAGS = $(AM_CPPFLAGS) $(POCKETCOIN_INCLUDES)
libpocketcoin_consensus_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libpocketcoin_consensus_a_SOURCES = \
  amount.h \
  arith_uint256.cpp \
  arith_uint256.h \
  consensus/merkle.cpp \
  consensus/merkle.h \
  consensus/params.h \
  consensus/validation.h \
  hash.cpp \
  hash.h \
  prevector.h \
  primitives/block.cpp \
  primitives/block.h \
  primitives/transaction.cpp \
  primitives/transaction.h \
  pubkey.cpp \
  pubkey.h \
  script/pocketcoinconsensus.cpp \
  script/interpreter.cpp \
  script/interpreter.h \
  script/script.cpp \
  script/script.h \
  script/script_error.cpp \
  script/script_error.h \
  serialize.h \
  span.h \
  tinyformat.h \
  uint256.cpp \
  uint256.h \
  utilstrencodings.cpp \
  utilstrencodings.h \
  version.h

# common: shared between pocketcoind, and pocketcoin-qt and non-server tools
libpocketcoin_common_a_CPPFLAGS = $(AM_CPPFLAGS) $(POCKETCOIN_INCLUDES)
libpocketcoin_common_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libpocketcoin_common_a_SOURCES = \
  base58.cpp \
  bech32.cpp \
  chainparams.cpp \
  coins.cpp \
  compressor.cpp \
  core_read.cpp \
  core_write.cpp \
  key.cpp \
  key_io.cpp \
  keystore.cpp \
  netaddress.cpp \
  netbase.cpp \
  policy/feerate.cpp \
  protocol.cpp \
  scheduler.cpp \
  script/descriptor.cpp \
  script/ismine.cpp \
  script/sign.cpp \
  script/standard.cpp \
  warnings.cpp \
  $(POCKETCOIN_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libpocketcoin_util_a_CPPFLAGS = $(AM_CPPFLAGS) $(POCKETCOIN_INCLUDES)
libpocketcoin_util_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libpocketcoin_util_a_SOURCES = \
    support/lockedpool.cpp \
    chainparamsbase.cpp \
    clientversion.cpp \
    compat/glibc_sanity.cpp \
    compat/glibcxx_sanity.cpp \
    compat/strnlen.cpp \
    fs.cpp \
    logging.cpp \
    random.cpp \
    rpc/protocol.cpp \
    support/cleanse.cpp \
    sync.cpp \
    threadinterrupt.cpp \
    util.cpp \
    utilmoneystr.cpp \
    utilstrencodings.cpp \
    utiltime.cpp \
    $(REINDEXER_H) \
    $(POCKETCOIN_CORE_H)

if GLIBC_BACK_COMPAT
libpocketcoin_util_a_SOURCES += compat/glibc_compat.cpp
AM_LDFLAGS += $(COMPAT_LDFLAGS)
endif

# cli: shared between pocketcoin-cli and pocketcoin-qt
libpocketcoin_cli_a_CPPFLAGS = $(AM_CPPFLAGS) $(POCKETCOIN_INCLUDES)
libpocketcoin_cli_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libpocketcoin_cli_a_SOURCES = \
    rpc/client.cpp \
    $(REINDEXER_H) \
    $(POCKETCOIN_CORE_H)

nodist_libpocketcoin_util_a_SOURCES = $(srcdir)/obj/build.h
#

# pocketcoind binary #
pocketcoind_SOURCES = pocketcoind.cpp $(REINDEXER_H)
pocketcoind_CPPFLAGS = $(AM_CPPFLAGS) $(POCKETCOIN_INCLUDES)
pocketcoind_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
pocketcoind_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
pocketcoind_SOURCES += pocketcoind-res.rc
endif

pocketcoind_LDADD = \
  $(LIBREINDEXER) \
  $(LIBPOCKETCOIN_SERVER) \
  $(LIBPOCKETCOIN_WALLET) \
  $(LIBPOCKETCOIN_COMMON) \
  $(LIBUNIVALUE) \
  $(LIBPOCKETCOIN_UTIL) \
  $(LIBPOCKETCOIN_ZMQ) \
  $(LIBPOCKETCOIN_CONSENSUS) \
  $(LIBPOCKETCOIN_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBLEVELDB_SSE42) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)

pocketcoind_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS) $(EVENT_PTHREADS_LIBS) $(EVENT_LIBS) $(ZMQ_LIBS)

# pocketcoin-cli binary #
pocketcoin_cli_SOURCES = pocketcoin-cli.cpp
pocketcoin_cli_CPPFLAGS = $(AM_CPPFLAGS) $(POCKETCOIN_INCLUDES) $(EVENT_CFLAGS)
pocketcoin_cli_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
pocketcoin_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
pocketcoin_cli_SOURCES += pocketcoin-cli-res.rc
endif

pocketcoin_cli_LDADD = \
  $(LIBPOCKETCOIN_CLI) \
  $(LIBUNIVALUE) \
  $(LIBPOCKETCOIN_UTIL) \
  $(LIBPOCKETCOIN_CRYPTO)

pocketcoin_cli_LDADD += $(BOOST_LIBS) $(CRYPTO_LIBS) $(EVENT_LIBS)
#

# pocketcoin-tx binary #
pocketcoin_tx_SOURCES = pocketcoin-tx.cpp
pocketcoin_tx_CPPFLAGS = $(AM_CPPFLAGS) $(POCKETCOIN_INCLUDES)
pocketcoin_tx_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
pocketcoin_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
pocketcoin_tx_SOURCES += pocketcoin-tx-res.rc
endif

pocketcoin_tx_LDADD = \
  $(LIBUNIVALUE) \
  $(LIBPOCKETCOIN_COMMON) \
  $(LIBPOCKETCOIN_UTIL) \
  $(LIBPOCKETCOIN_CONSENSUS) \
  $(LIBPOCKETCOIN_CRYPTO) \
  $(LIBSECP256K1)

pocketcoin_tx_LDADD += $(BOOST_LIBS) $(CRYPTO_LIBS)
#

# pocketcoinconsensus library #
if BUILD_POCKETCOIN_LIBS
include_HEADERS = script/pocketcoinconsensus.h
libpocketcoinconsensus_la_SOURCES = $(crypto_libpocketcoin_crypto_base_a_SOURCES) $(libpocketcoin_consensus_a_SOURCES)

if GLIBC_BACK_COMPAT
  libpocketcoinconsensus_la_SOURCES += compat/glibc_compat.cpp
endif

libpocketcoinconsensus_la_LDFLAGS = $(AM_LDFLAGS) -no-undefined $(RELDFLAGS)
libpocketcoinconsensus_la_LIBADD = $(LIBSECP256K1)
libpocketcoinconsensus_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(builddir)/obj -I$(srcdir)/secp256k1/include -DBUILD_POCKETCOIN_INTERNAL
libpocketcoinconsensus_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)

endif
#

CTAES_DIST =  crypto/ctaes/bench.c
CTAES_DIST += crypto/ctaes/ctaes.c
CTAES_DIST += crypto/ctaes/ctaes.h
CTAES_DIST += crypto/ctaes/README.md
CTAES_DIST += crypto/ctaes/test.c

CLEANFILES = $(EXTRA_LIBRARIES)

CLEANFILES += *.gcda *.gcno
CLEANFILES += compat/*.gcda compat/*.gcno
CLEANFILES += consensus/*.gcda consensus/*.gcno
CLEANFILES += crypto/*.gcda crypto/*.gcno
CLEANFILES += policy/*.gcda policy/*.gcno
CLEANFILES += primitives/*.gcda primitives/*.gcno
CLEANFILES += script/*.gcda script/*.gcno
CLEANFILES += support/*.gcda support/*.gcno
CLEANFILES += univalue/*.gcda univalue/*.gcno
CLEANFILES += wallet/*.gcda wallet/*.gcno
CLEANFILES += wallet/test/*.gcda wallet/test/*.gcno
CLEANFILES += zmq/*.gcda zmq/*.gcno
CLEANFILES += obj/build.h

EXTRA_DIST = $(CTAES_DIST)


config/pocketcoin-config.h: config/stamp-h1
	@$(MAKE) -C $(top_builddir) $(subdir)/$(@)
config/stamp-h1: $(top_srcdir)/$(subdir)/config/pocketcoin-config.h.in $(top_builddir)/config.status
	$(AM_V_at)$(MAKE) -C $(top_builddir) $(subdir)/$(@)
$(top_srcdir)/$(subdir)/config/pocketcoin-config.h.in:  $(am__configure_deps)
	$(AM_V_at)$(MAKE) -C $(top_srcdir) $(subdir)/config/pocketcoin-config.h.in

clean-local:
	-$(MAKE) -C secp256k1 clean
	-$(MAKE) -C univalue clean
	-rm -f leveldb/*/*.gcda leveldb/*/*.gcno leveldb/helpers/memenv/*.gcda leveldb/helpers/memenv/*.gcno
	-rm -f config.h
	-rm -rf test/__pycache__

.rc.o:
	@test -f $(WINDRES)
	## FIXME: How to get the appropriate modulename_CPPFLAGS in here?
	$(AM_V_GEN) $(WINDRES) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) -DWINDRES_PREPROC -i $< -o $@

check-symbols: $(bin_PROGRAMS)
if GLIBC_BACK_COMPAT
	@echo "Checking glibc back compat..."
	$(AM_V_at) READELF=$(READELF) CPPFILT=$(CPPFILT) $(top_srcdir)/contrib/devtools/symbol-check.py < $(bin_PROGRAMS)
endif

check-security: $(bin_PROGRAMS)
if HARDEN
	@echo "Checking binary security..."
	$(AM_V_at) READELF=$(READELF) OBJDUMP=$(OBJDUMP) $(top_srcdir)/contrib/devtools/security-check.py < $(bin_PROGRAMS)
endif

%.pb.cc, %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(<D) $<

if EMBEDDED_LEVELDB
include Makefile.leveldb.include
endif

# if ENABLE_TESTS
# include Makefile.test.include
# endif

# if ENABLE_BENCH
# include Makefile.bench.include
# endif

if ENABLE_QT
include Makefile.qt.include
endif

# if ENABLE_QT_TESTS
# include Makefile.qttest.include
# endif
