#pragma once

void m_plug_init();
void reloadDLL();
void m_plug_update();
void m_plug_shutdown();
void m_post_reload(void* s);
void* m_pre_reload();