#include "agent_hook.h"
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

unsigned int la_version(unsigned int version)
{
    if (version == 0)
        return version;
    else
        return LAV_CURRENT;
}

char *la_objsearch(const char *name, uintptr_t *cookie, unsigned int flag)
{
    UNUSED(cookie);
    UNUSED(flag);

    if (is_preload(name) && (allowed_preload(name) == false))
        return NULL;
    else
        return (char *)name;
}

const char *LD_PRELOAD_ENV_VAR = "LD_PRELOAD";
const char *LD_SO_PRELOAD_PATH = "/etc/ld.so.preload";
const char *LIB_PRELOAD_WHITELIST_CONFIG_PATH = "/etc/fleetagent/ld_preload.allow";

bool is_preload(const char *object)
{
    bool in_ld_preload = env_variable_contains_object(object, LD_PRELOAD_ENV_VAR);
    bool in_ld_so_preload = file_contains_object(object, LD_SO_PRELOAD_PATH);

    return (in_ld_preload || in_ld_so_preload);
}

bool allowed_preload(const char *object)
{
    return file_contains_object(object, LIB_PRELOAD_WHITELIST_CONFIG_PATH);
}

bool env_variable_contains_object(const char *object, const char *env_variable)
{
    bool contains_object = false;
    const char *ld_preload = getenv(env_variable);

    if (ld_preload != NULL)
        contains_object = (strstr(ld_preload, object) != NULL);

  return contains_object;
}

bool file_contains_object(const char *object, const char *specified_file)
{
    bool contains_object = false;

    int fd = 0;
    struct stat file_info = {0};
    size_t size = 0;

    if ((fd = open(specified_file, O_RDONLY)) >= 0)
    {
        if (fstat(fd, &file_info) == 0)
        {
            size = file_info.st_size;
            char mapped_file[size];

            if (read(fd, mapped_file, size) >= 0)
            {
                contains_object = (strstr(mapped_file, object) != NULL);
            }
        }
    }
    if (fd)
        close(fd);
    return contains_object;
}
