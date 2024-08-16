# osquery-ebpf-extensions

## Using icmp_events_dbless extension
    $ git clone https://github.com/ansbaby7/osquery-ebpf-extensions.git
    $ cd osquery-ebpf-extensions/go/icmp_events_dbless
    $ go mod tidy
    $ go generate .

In a second terminal tab, start osqueryi with extensions enabled and query the extensions_flags table to get the socket path.

    $ sudo osqueryi --nodisable_extensions
    osquery> select value from osquery_flags where name = 'extensions_socket';
Now run the following command with the socket path we got from osquery. It will be something like

    $ go run -exec sudo . --socket /root/.osquery/shell.em

You can also specify an interface to attach the eBPF XDP program using the --iface flag. Default is loopback (lo).