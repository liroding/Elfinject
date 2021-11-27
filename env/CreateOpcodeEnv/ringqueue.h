/**@brief 仿照linux kfifo写的ring buffer
 *@atuher Anker  date:2013-12-18
* ring_buffer.h
 * */

#ifndef KFIFO_HEADER_H
#define KFIFO_HEADER_H

#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

//判断x是否是2的次方
#define is_power_of_2(x) ((x) != 0 && (((x) & ((x) - 1)) == 0))
//取a和b中最小值
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif
