## Client-Server Protocol Description

### Currently Available Endpoints

- **Task**
    - ``task create`` : creates task associated with certain project (also it sends notifications to other clients who
      listen to certain events from ``project X``).
    - ``task get_all`` : basically gets all tasks associated with certain project (probably in future pagination will be
      added) and subscribes client to resource ``project<project_id>`` (since most likely callee is interested in other
      stuff that is going on with project ``<project_id>``).
    - ``task edit``: not ready to use yet but behaviour is going to be really similar to ``task create``.

```c++
/* Example of using it by client */
#include <nlohmann/json.hpp>
#include "structures.h"
#include "serialization.h"

using nlohmann::json;

std::vector<Task> get_all_project_tasks(int project_id) {
  // DTO is created just for future enhancements (adding pagination etc.)
  using DTO = TaskGetAllDTO;
  DTO dto = {project_id};
  // first initializer-list argument is endpoint name itself
  RequestFormat<DTO> get_all_req = {
          "task get_all",
          dto
  };
  json json_req = get_all_req; // no magic, serialization logic is in header
  // Network-specific manipulations
  socket.async_write_some(...);
  // Here it ends
}
```

- **Project**
    - ``project create``

## Subscriptions

Subscription is just fancy name for abstraction that denotes resource that notifies about its state changes all
interested clients.

For instance, ``project<project_id>`` is a good example of such a resource. There are clients who are interested in its
relevant state (employees who are working on this project) and there are modifications that must be sent to all clients
as quickly as possible (creations of new tasks, changes of the old ones).

### Currently Available Subscriptions:

- ``project<project_id>`` -- project information subscription, all information changed will be delivered to clients
