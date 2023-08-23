#include <libxfce4ui/libxfce4ui.h>
#include "xfce-polkit-listener.h"

int main(int argc, char *argv[])
{
	PolkitAgentListener *listener;
	PolkitSubject* subject;
	GError* err = NULL;
	int rc = 0;

	gtk_init(&argc, &argv);

	listener = xfce_polkit_listener_new();
	pid_t pid_of_caller;
	pid_of_caller = getppid();
	subject = polkit_unix_process_new_for_owner(pid_of_caller, 0, getuid ());

	g_assert(subject != NULL);
	g_assert(polkit_unix_process_get_pid(POLKIT_UNIX_PROCESS (subject)) == pid_of_caller);
	g_assert(polkit_unix_process_get_uid(POLKIT_UNIX_PROCESS (subject)) >= 0);
	g_assert(polkit_unix_process_get_start_time(POLKIT_UNIX_PROCESS (subject)) > 0);

	if(!polkit_agent_listener_register(listener,
					   POLKIT_AGENT_REGISTER_FLAGS_NONE,
					   subject, NULL, NULL, &err)) {
		xfce_dialog_show_error(NULL, err, "XFCE PolicyKit Agent");
		rc = 1;
	} else {
		gtk_main();
	}

	g_object_unref(listener);
	g_object_unref(subject);
	return rc;
}
