# 0 "test.cpp"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "test.cpp"
# 16 "test.cpp"
plug_hello = (plug_hello_t)findsymbol(handle, "plug_hello"); if (plug_hello == NULL) { fprintf(stderr, "ERROR: could not find %s symbol in %s.\n", "plug_hello", libplug); } plug_init = (plug_init_t)findsymbol(handle, "plug_init"); if (plug_init == NULL) { fprintf(stderr, "ERROR: could not find %s symbol in %s.\n", "plug_init", libplug); } plug_update = (plug_update_t)findsymbol(handle, "plug_update"); if (plug_update == NULL) { fprintf(stderr, "ERROR: could not find %s symbol in %s.\n", "plug_update", libplug); } plug_pre_reload = (plug_pre_reload_t)findsymbol(handle, "plug_pre_reload"); if (plug_pre_reload == NULL) { fprintf(stderr, "ERROR: could not find %s symbol in %s.\n", "plug_pre_reload", libplug); } plug_post_reload = (plug_post_reload_t)findsymbol(handle, "plug_post_reload"); if (plug_post_reload == NULL) { fprintf(stderr, "ERROR: could not find %s symbol in %s.\n", "plug_post_reload", libplug); }




plug_hello_t plug_hello; plug_init_t plug_init; plug_update_t plug_update; plug_pre_reload_t plug_pre_reload; plug_post_reload_t plug_post_reload;
