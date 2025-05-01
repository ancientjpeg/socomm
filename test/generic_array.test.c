#include <assert.h>
#include <socomm/generic_array.h>
#include <stdint.h>
#include <stdlib.h>

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

  socomm_array_reserve(int_arr, 50);
  assert(socomm_array_length(int_arr) == 0);
  assert(socomm_array_capacity(int_arr) >= 50);

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

    assert(socomm_array_contains(int_arr, &data[i], NULL));
    assert(socomm_array_find(int_arr, &data[i], NULL) != NULL);
    assert(socomm_array_find(int_arr, &data[i], NULL)
           == ((uint8_t *)socomm_array_at(int_arr, 0) + (i * sizeof(int))));
  }

  assert(socomm_array_length(int_arr) == data_count);
  assert(socomm_array_capacity(int_arr) >= data_count);

  socomm_array_destroy(&int_arr);
}

struct custom_data {
  int  id;
  char other_data[32];
};

struct custom_data gen_custom_data(int id)
{
  struct custom_data d;
  d.id          = id;
  const int len = sizeof(d.other_data) / sizeof(d.other_data[0]);

  for (size_t i = 0; i < len; ++i) {
    d.other_data[i] = rand();
  }

  return d;
}

typedef struct custom_data custom_data;

int custom_data_comparator(const void *a, const void *b, size_t sz)
{
  return ((custom_data *)a)->id != ((custom_data *)b)->id;
}

void test_custom_comp()
{
  const int     ARRAY_SIZE = 3;
  socomm_array *data_arr
      = socomm_array_create_reserve(sizeof(custom_data), ARRAY_SIZE);
  assert(data_arr != NULL);

  for (size_t i = 0; i < ARRAY_SIZE; ++i) {
    custom_data d = gen_custom_data(i);
    socomm_array_push_back(data_arr, &d);
  }

  custom_data *el_1        = socomm_array_at(data_arr, 1);
  custom_data  el_1_backup = *el_1;
  custom_data  dummy_el_1  = gen_custom_data(1);

  assert(el_1->id == 1);
  assert(socomm_array_index_of(data_arr, el_1, NULL) == 1);
  assert(socomm_array_index_of(data_arr, el_1, custom_data_comparator) == 1);
  assert(socomm_array_index_of(data_arr, &dummy_el_1, custom_data_comparator)
         == 1);
  assert(socomm_array_index_of(data_arr, &dummy_el_1, NULL)
         == socomm_array_length(data_arr));
  assert(socomm_array_find(data_arr, &dummy_el_1, custom_data_comparator)
         == el_1);

  socomm_array_push_back(data_arr, &dummy_el_1);

  assert(socomm_array_length(data_arr) == ARRAY_SIZE + 1);
  socomm_array_purge(data_arr, el_1, custom_data_comparator);
  assert(socomm_array_length(data_arr) == ARRAY_SIZE - 1);

  socomm_array_insert_at(data_arr, &el_1_backup, 1);
  assert(socomm_array_index_of(data_arr, &el_1_backup, NULL) == 1);
  socomm_array_push_back(data_arr, &dummy_el_1);
  assert(socomm_array_index_of(data_arr, &dummy_el_1, NULL) == 3);
  assert(socomm_array_index_of(data_arr, &dummy_el_1, custom_data_comparator)
         == 1);

  assert(socomm_array_length(data_arr) == ARRAY_SIZE + 1);
  socomm_array_purge(data_arr, &el_1_backup, NULL);
  assert(socomm_array_length(data_arr) == ARRAY_SIZE);
  assert(socomm_array_index_of(data_arr, &dummy_el_1, custom_data_comparator)
         != socomm_array_length(data_arr));
  assert(socomm_array_index_of(data_arr, &el_1_backup, NULL)
         == socomm_array_length(data_arr));
}

int main()
{
  test_create_reserve();
  test_insertion_and_removal();
  test_getters_and_search();
  test_custom_comp();

  return 0;
}
