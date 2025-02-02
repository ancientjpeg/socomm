#include <assert.h>
#include <socomm/errors.h>
#include <socomm/ledger.h>

int main()
{
  socomm_ledger *ledger        = socomm_ledger_create();

  const size_t   TEST_ARR_SIZE = 5;
  uuid4_t        uuids[TEST_ARR_SIZE];
  uint16_t       ports[TEST_ARR_SIZE];

  uuid4_state_t  uuid_state;
  uuid4_seed(&uuid_state);

  uuid4_t dummy_uuid;
  uuid4_gen(&uuid_state, &dummy_uuid);

  for (size_t i = 0; i < TEST_ARR_SIZE; ++i) {
    uuid4_gen(&uuid_state, &uuids[i]);
    ports[i]   = i + 1;

    int add_rc = socomm_ledger_add_entry(ledger, uuids[i], ports[i]);

    assert(add_rc == SOCOMM_SUCCESS);
  }

  for (size_t i = 0; i < TEST_ARR_SIZE; ++i) {
    assert(socomm_ledger_entry_exists(ledger, uuids[i]));
    assert(socomm_ledger_add_entry(ledger, uuids[i], 0)
           == SOCOMM_ALREADY_EXISTS);
    assert(socomm_ledger_add_entry(ledger, dummy_uuid, ports[i])
           == SOCOMM_PORT_IN_USE);
  }

  const size_t remove_idx = 2;
  assert(socomm_ledger_remove_entry(ledger, dummy_uuid)
         == SOCOMM_DOES_NOT_EXIST);

  int remove_rc = socomm_ledger_remove_entry(ledger, uuids[remove_idx]);
  assert(remove_rc == SOCOMM_SUCCESS);

  assert(!socomm_ledger_entry_exists(ledger, uuids[remove_idx]));

  assert(socomm_ledger_add_entry(ledger, uuids[remove_idx], ports[remove_idx])
         == SOCOMM_SUCCESS);
}
