<?hh
/**
 * @author sunjianfei <sunjianfei@xiaomi.com>
 * port lz4 to hhvm
 * @copyright (c) 2015~2017 Xiaomi.LTD
 * @license http://gnu.org 
 */
<<__Native>>
function lz4_compress(string $data, bool $high=false, string $extra = NULL): mixed;
<<__Native>>
function lz4_uncompress(string $data, int $maxsize = -1, int $offset = -1): mixed;

