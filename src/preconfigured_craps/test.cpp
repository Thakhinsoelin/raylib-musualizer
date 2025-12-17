
#define LIST_OF_PLUGS \
    PLUG(plug_hello) \
    PLUG(plug_init) \
    PLUG(plug_update) \
    PLUG(plug_pre_reload) \
    PLUG(plug_post_reload)

#define PLUG(name) \
    name = (name##_t)findsymbol(handle, #name); \
    if (name == NULL) { \
        fprintf(stderr, "ERROR: could not find %s symbol in %s.\n", \
                #name, libplug); \
    }

LIST_OF_PLUGS

#undef PLUG

#define PLUG(name) name##_t name;
LIST_OF_PLUGS
#undef PLUG
