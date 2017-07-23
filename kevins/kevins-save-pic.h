/* 照抄 tsark.h 包含的头文件 start */
#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <limits.h>

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif

#include <errno.h>

#ifndef _WIN32
#include <signal.h>
#endif

#ifdef HAVE_LIBCAP
# include <sys/capability.h>
#endif

#ifndef HAVE_GETOPT_LONG
#include "wsutil/wsgetopt.h"
#endif

#include <glib.h>

#include <epan/exceptions.h>
#include <epan/epan-int.h>
#include <epan/epan.h>

#include <wsutil/clopts_common.h>
#include <wsutil/cmdarg_err.h>
#include <wsutil/crash_info.h>
#include <wsutil/filesystem.h>
#include <wsutil/file_util.h>
#include <wsutil/privileges.h>
#include <wsutil/report_err.h>
#include <ws_version_info.h>
#include <wiretap/wtap_opttypes.h>
#include <wiretap/pcapng.h>

#include "globals.h"
#include <epan/timestamp.h>
#include <epan/packet.h>
#ifdef HAVE_LUA
#include <epan/wslua/init_wslua.h>
#endif
#include "frame_tvbuff.h"
#include <epan/disabled_protos.h>
#include <epan/prefs.h>
#include <epan/column.h>
#include <epan/print.h>
#include <epan/addr_resolv.h>
#ifdef HAVE_LIBPCAP
#include "ui/capture_ui_utils.h"
#endif
#include "ui/util.h"
#include "ui/ui_util.h"
#include "ui/decode_as_utils.h"
#include "ui/cli/tshark-tap.h"
#include "ui/tap_export_pdu.h"
#include "register.h"
#include "filter_files.h"
#include <epan/epan_dissect.h>
#include <epan/tap.h>
#include <epan/stat_tap_ui.h>
#include <epan/conversation_table.h>
#include <epan/srt_table.h>
#include <epan/rtd_table.h>
#include <epan/ex-opt.h>
#include <epan/exported_pdu.h>
/*  */

#if defined(HAVE_HEIMDAL_KERBEROS) || defined(HAVE_MIT_KERBEROS)
#include <epan/asn1.h>
#include <epan/dissectors/packet-kerberos.h>
#endif

#include "capture_opts.h"

#include "caputils/capture-pcap-util.h"

#ifdef HAVE_LIBPCAP
#include "caputils/capture_ifinfo.h"
#ifdef _WIN32
#include "caputils/capture-wpcap.h"
#include <wsutil/os_version_info.h>
#include <wsutil/unicode-utils.h>
#endif /* _WIN32 */
#include <capchild/capture_session.h>
#include <capchild/capture_sync.h>
#include <capture_info.h>
#endif /* HAVE_LIBPCAP */
#include "log.h"
#include <epan/funnel.h>

#include <wsutil/str_util.h>
#include <wsutil/utf8_entities.h>

#ifdef HAVE_EXTCAP
#include "extcap.h"
#endif

#ifdef HAVE_PLUGINS
#include <wsutil/plugins.h>
#endif
/* 照抄 tsark.h 包含的头文件 stop */

/* 新增的头文件 start */
#include <epan/tvbuff-int.h>
#include <kevins/kevins-file.h>
/* 新增的头文件 stop */

// 图片根目录
#define KEVINS_PIC_FLODER_NAME "d:\\kevins_save_pic\\" 
// 图片名前缀
#define KEVINS_PIC_FILE_NAME "save_pic"

// 全局变量 标记数据包是否为图片
extern int kevins_g_is_pic;
// 全局变量 标记数据包来源IP
extern char kevins_g_src_ip[ KEVINS_MAXPATHLEN ];

// 图片类型
#define KEVINS_PIC_JPG 1
#define KEVINS_PIC_PNG 2
#define KEVINS_PIC_GIF 3

// 主要功能函数
void kevins_save_pic(epan_dissect_t * edt);