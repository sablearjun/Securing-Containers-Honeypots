#ifndef FLEET_GTFOBINS_RULES_H
#define FLEET_GTFOBINS_RULES_H

#include "../common/EdrDataTypes.h"

bool wget_download_tmp(const ProcessEvent &process_event, Event &rule_event);
bool cpulimit_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool dmesg_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool docker_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool dpkg_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool env_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool find_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool flock_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool apt_get_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool apt_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool ash_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool awk_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool bundler_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool busctl_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool byebug_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool cpan_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool ionice_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool journalctl_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool ksh_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool less_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool logsave_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool ltrace_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool lua_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool man_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool mawk_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool mysql_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool nano_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool nice_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool nmap_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool nohup_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool nroff_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool nsenter_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool perl_spawn_shell(const ProcessEvent &process_event, Event &rule_event);

//
bool git_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool ftp_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool gawk_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool gcc_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool gdb_spawn_shell(const ProcessEvent &process_event, Event &rule_event);

bool tar_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool taskset_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool time_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool timeout_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool tmux_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool watch_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool vi_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool vim_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool view_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool xargs_spawn_shell(const ProcessEvent &process_event, Event &rule_event);

bool zip_spawn_shell(const ProcessEvent &process_event, Event &rule_event);

bool strace_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool stdbuf_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool startstopdaemon_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool split_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool ssh_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool sqlite3_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool setarch_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool sed_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool slsh_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool socat_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool script_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool screen_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool pry_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool runmailcap_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool puppet_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool rake_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool rpm_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool rlwrap_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool ruby_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool rsync_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool scp_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool pic_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool php_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool unshare_spawn_shell(const ProcessEvent &process_event, Event &rule_event);

// Arjun
bool at_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
// bool bash_spawn_shell(const ProcessEvent& process_event, Event& rule_event);
bool capsh_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool cowsay_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool crash_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool csh_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
// bool dash_spawn_shell(const ProcessEvent& process_event, Event& rule_event);
bool ed_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool emacs_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool ex_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool expect_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool facter_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool ghc_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool ghci_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool hping_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool gtester_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool gimp_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool irb_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool jjs_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool mail_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool make_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool more_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool nawk_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool node_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
// bool pico_spawn_shell(const ProcessEvent &process_event, Event &rule_event);

bool zypper_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool zsh_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool valgrind_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool top_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool tclsh_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool service_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool rview_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool run_parts_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool python_spawn_shell(const ProcessEvent &process_event, Event &rule_event);
bool psql_spawn_shell(const ProcessEvent &process_event, Event &rule_event);

#endif // FLEET_GTFOBINS_RULES_H