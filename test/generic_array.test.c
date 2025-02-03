#include <assert.h>
#include <socomm/generic_array.h>

int main()
{
  const int     ARRAY_SIZE = 3;
  socomm_array *int_arr = socomm_array_create_reserve(sizeof(int), ARRAY_SIZE);
  assert(int_arr != NULL);
  assert(socomm_array_count(int_arr) == 0);
  assert(socomm_array_capacity(int_arr) >= 3);

  socomm_array_destroy(&int_arr);
  assert(int_arr == NULL);
  return 0;
}
