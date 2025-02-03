#include <assert.h>
#include <socomm/generic_array.h>
#include <stdint.h>

void test_create_reserve()
{
  const int     ARRAY_SIZE = 3;
  socomm_array *int_arr = socomm_array_create_reserve(sizeof(int), ARRAY_SIZE);
  assert(int_arr != NULL);
  assert(socomm_array_length(int_arr) == 0);
  assert(socomm_array_capacity(int_arr) >= 3);

  socomm_array_destroy(&int_arr);
  assert(int_arr == NULL);
}

void test_insertion_and_removal()
{

  socomm_array *int_arr = socomm_array_create(sizeof(int));
  assert(socomm_array_length(int_arr) == 0);
  assert(socomm_array_capacity(int_arr) == 0);

  int el = 0;
  socomm_array_push_back(int_arr, &el);
  assert(*(int *)socomm_array_at(int_arr, 0) == 0);

  el = 2;
  socomm_array_push_back(int_arr, &el);
  assert(*(int *)socomm_array_at(int_arr, 1) == 2);

  el = 1;
  socomm_array_insert_at(int_arr, &el, 1);
  assert(*(int *)socomm_array_at(int_arr, 1) == 1);
  assert(*(int *)socomm_array_at(int_arr, 2) == 2);

  assert(socomm_array_length(int_arr) == 3);
  assert(socomm_array_capacity(int_arr) >= 3);

  socomm_array_remove(int_arr, 1);
  assert(*(int *)socomm_array_at(int_arr, 1) == 2);

  socomm_array_pop_back(int_arr);
  assert(*(int *)socomm_array_at(int_arr, 0) == 0);
  assert(socomm_array_length(int_arr) == 1);

  socomm_array_remove(int_arr, 0);
  assert(socomm_array_length(int_arr) == 0);

  socomm_array_destroy(&int_arr);
}

void test_getters_and_search()
{

  socomm_array *int_arr = socomm_array_create(sizeof(int));
  assert(int_arr != NULL);
  assert(socomm_array_capacity(int_arr) == 0);
  assert(socomm_array_length(int_arr) == 0);

  const int reserve_amt = 150;
  socomm_array_reserve(int_arr, reserve_amt);
  assert(socomm_array_capacity(int_arr) >= reserve_amt);
  assert(socomm_array_length(int_arr) == 0);

#define data_count 3
  assert(data_count <= reserve_amt);

  int data[data_count] = {15, 18, 5085};
  for (size_t i = 0; i < data_count; ++i) {
    socomm_array_push_back(int_arr, &data[i]);
    assert(socomm_array_length(int_arr) == i + 1);
    assert(*(int *)socomm_array_at(int_arr, i) == data[i]);

    assert(socomm_array_contains(int_arr, &data[i]));
    assert(socomm_array_find(int_arr, &data[i]) != NULL);
    assert(socomm_array_find(int_arr, &data[i])
           == ((uint8_t *)socomm_array_at(int_arr, 0) + (i * sizeof(int))));
  }

  assert(socomm_array_length(int_arr) == data_count);
  assert(socomm_array_capacity(int_arr) >= data_count);

  socomm_array_destroy(&int_arr);
}

int main()
{
  test_create_reserve();
  test_insertion_and_removal();
  test_getters_and_search();

  return 0;
}
