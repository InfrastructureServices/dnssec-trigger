#TODO:

## Rewrite parts of the python scipt into C
  * handle\_update\_all:
    * `self.run_update_global_forwarders()` will translate directly into filter only default connections
  * Generate doxygen documentation

## Unit testing
  * Write unit tests for the whole connection/string API

## Testing
  * Use Vagrant for more complicated testing scenarios (e.g. creation of network namespaces, running Bind as
  a authoritative or recursive DNS server)
  * Write fake unbound-control which will collect results of the probing process
  * Try to incorporate BDD: http://pythonhosted.org/behave/
