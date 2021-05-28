//
// Created by egor on 28.05.2021.
//
#include <vector>
#include "app_db_connection.hpp"
#include "structures.h"

namespace comment_service {
Comment create_comment(CommentDTO const &commentDTO) {
  // WTF??? Weird API, no id is returned, no task itself is returned
  get_app_db().add_comment(commentDTO.task_id, commentDTO.task_id,
                           commentDTO.comment);
  return {};
}
}  // namespace comment_service
