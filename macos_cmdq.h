// macos_cmdq.h
#ifndef MACOS_CMDQ_H
#define MACOS_CMDQ_H

#ifdef __APPLE__
// Installs a local macOS keyboard monitor for Command+Q
void install_cmdq_monitor(void *win);
#endif

#endif