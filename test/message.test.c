#include "socomm/socomm.h"

int main()
{
  uuid4_state_t uuid_state;
  uuid4_t       uuid;
  uuid4_seed(&uuid_state);
  uuid4_gen(&uuid_state, &uuid);

  socomm_header   header      = socomm_header_init(9350, uuid);

  const char      test_data[] = "TEST DATA STRING";
  socomm_message *msg         = socomm_message_create(header,
                                              SOCOMM_MESSAGE_TYPES[0],
                                              test_data,
                                              sizeof(test_data));
  assert(msg != NULL);

  const char *msg_data = (const char *)socomm_message_data(msg);
  assert(msg_data != NULL);
  assert(socomm_message_data_size(msg) == sizeof(test_data));

  assert(strcmp(msg_data, test_data) == 0);

  socomm_message_destroy(&msg);
  assert(msg == NULL);

  socomm_message *empty_msg
      = socomm_message_create(header, SOCOMM_MESSAGE_TYPES[0], NULL, 0);
  assert(empty_msg != NULL);
  assert(socomm_message_data(empty_msg) == NULL);
  assert(socomm_message_data_size(empty_msg) == 0);

  /* test non-existant command */
  assert(socomm_message_create(header, "NOTCMD", NULL, 0) == NULL);
  /* test out-of-bounds command */
  assert(socomm_message_create(header, "TOO_LONG_STRING", NULL, 0) == NULL);
}
