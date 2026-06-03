#ifndef IDENTITYHASH_HPP
#define IDENTITYHASH_HPP
#include <cstddef>
namespace kitserov
{
  struct IdentityHash
  {
    size_t operator()(int key) const
    {
      return static_cast< size_t >(key);
    }
  };
}
#endif
