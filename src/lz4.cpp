/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-2013 Facebook, Inc. (http://www.facebook.com)     |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
 */

#include "hphp/runtime/base/base-includes.h"

/* lz4 */
#include "lz4/lz4.h"
#include "lz4/lz4hc.h"

namespace HPHP {

    static String HHVM_FUNCTION(lz4_compress, const String& data, bool high, const String& extra) {

        char *output = NULL;
        int output_len, data_len;
        int extra_len = -1;
        int offset = 0;

        extra_len = extra.length();
        if (extra && extra_len > 0) {
            offset = extra_len;
        } else {
            offset = sizeof (int);
        }
        data_len = data.length();
        output = (char *) malloc(LZ4_compressBound(data_len) + offset);

        if (!output) {
            raise_error("lz4_compress : memory error");
        }
        if (extra && extra_len > 0) {
            memcpy(output, (const void *) extra.c_str(), offset);
        } else {
            /* Set the data length */
            memcpy(output, &data_len, offset);
        }

        if (high) {
            output_len = LZ4_compressHC(data.c_str(), output + offset, data_len);
        } else {
            output_len = LZ4_compress(data.c_str(), output + offset, data_len);
        }
        if (output_len <= 0) {
            raise_error("lz4_compress : compress error ,%s is not compressable",data.c_str());
        }
        CopyStringMode mode = CopyString;
        String compress = String(output, output_len + offset, mode);
        free(output);
        return compress;
    }

    static String HHVM_FUNCTION(lz4_uncompress, const String& data, long max_size, long offset) {

        int output_len, data_size;
        char *output;
        if (max_size > 0) {
            data_size = max_size;
            if (!offset) {
                offset = sizeof (int);
            }
        } else {
            /* Get data length */
            offset = sizeof (int);
            memcpy(&data_size, data.c_str(), offset);
        }
        if (data_size < 0) {
            raise_error("lz4_uncompress : allocate size error");
        }
        output = (char *) malloc(data_size + 1);
        if (!output) {
            raise_error("lz4_uncompress : memory error");
        }
        output_len = LZ4_decompress_safe(data.c_str() + offset,
                output,
                data.length() - offset,
                data_size);
        if (output_len <= 0) {
            raise_error("lz4_uncompress : data error, %s is not uncompressable",data.c_str());
        }
        CopyStringMode mode = CopyString;
        String uncompress = String(output, output_len, mode);
        free(output);
        return uncompress;

    }

    static class lz4Extension : public Extension {
    public:

        lz4Extension() : Extension("lz4") {
        }

        virtual void moduleInit() {
            HHVM_FE(lz4_compress);
            HHVM_FE(lz4_uncompress);
            loadSystemlib();
        }
    } s_lz4_extension;

    HHVM_GET_MODULE(lz4)

} // namespace HPHP
