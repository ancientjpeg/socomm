#include <assert.h>
#include <socomm/errors.h>
#include <socomm/ledger.h>

int main()
{
  socomm_ledger_t *ledger        = socomm_ledger_create();

  const size_t     TEST_ARR_SIZE = 5;
  uuid4_t          uuids[TEST_ARR_SIZE];
  uint16_t         ports[TEST_ARR_SIZE];

  uuid4_state_t    uuid_state;
  uuid4_seed(&uuid_state);

  for (size_t i = 0; i < TEST_ARR_SIZE; ++i) {
    uuid4_gen(&uuid_state, &uuids[i]);
    ports[i]   = i + 1;

    int add_rc = socomm_ledger_add_entry(ledger, uuids[i], ports[i]);

    assert(add_rc == SOCOMM_SUCCESS);
    assert(socomm_ledger_entry_exists(ledger, uuids[i]));
  }
}
