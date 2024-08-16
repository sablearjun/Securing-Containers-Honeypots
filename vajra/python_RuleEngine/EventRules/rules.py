def ash_spawn_shell(output):
    process_event = output['row_data']
    process_tree = output['process_tree']
    if ((process_event['path'] in ["/usr/bin/dash","/usr/bin/zsh", '/usr/lib/postgresql/14/bin/postgres']) | (process_event['path'][-2:] == 'sh')):

        ss = "[" + str(process_event['path']) + " (pid: " + str(process_event['pid']) + ")]"
        print(ss)
        return True, ss

    ss = ''
    return False, ss
