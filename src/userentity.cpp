/**
 * User entity handler to save info.
 * @author Daniel Victoriano
 * @version 1.0
 */
#include "userentity.h"

UserEntity::UserEntity()
{
}

inline bool UserEntity::operator==(const UserEntity &other)
{
   return username == other.username;
}
