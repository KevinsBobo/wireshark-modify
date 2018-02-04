// dllmain.cpp : 定义 DLL 应用程序的入口点。


#include "config.h"
#include <epan/packet.h>
//#include "config.h"

#include <gmodule.h>

#include "moduleinfo.h"

/* plugins are DLLs */
#define WS_BUILD_DLL
#include "ws_symbol_export.h"
#define FOO_PORT 1234

static int proto_foo = -1;

WS_DLL_PUBLIC void	col_set_str(column_info *cinfo, const gint col, const gchar * str);

void proto_register_foo(void)
{
	proto_foo = proto_register_protocol(
		"FOO Protocol", /* name       */
		"FOO",      /* short name */
		"foo"       /* abbrev     */
		);
}

static int dissect_foo(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree, void* data)
{
	col_set_str(pinfo->cinfo, COL_PROTOCOL, "FOO");
	/* Clear out stuff in the info column */
	col_clear(pinfo->cinfo, COL_INFO);
	return 0;
}


void proto_reg_handoff_foo(void)
{
	static dissector_handle_t foo_handle;
	foo_handle = create_dissector_handle(dissect_foo, proto_foo);
	dissector_add_uint("udp.port", FOO_PORT, foo_handle);
}

__declspec(dllexport) void plugin_register(void)
{
	proto_register_foo();
}

__declspec(dllexport) const gchar version[] = VERSION;

__declspec(dllexport) void plugin_reg_handoff(void)
{
	proto_reg_handoff_foo();
}
