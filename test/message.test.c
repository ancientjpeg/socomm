#include "socomm/header.h"
#include "socomm/header_serde.h"
#include "socomm/uuid/uuid.h"
#include "test_helpers.h"
#include <string.h>

void test_messages()
{
  socomm_header   header      = get_test_header();

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

void test_serde()
{

  socomm_header   header      = get_test_header();

  const char      test_data[] = "TEST DATA STRING";
  const size_t    data_size   = strlen(test_data) + 1;
  socomm_message *msg_out     = socomm_message_create(header,
                                                  SOCOMM_MESSAGE_TYPES[0],
                                                  test_data,
                                                  sizeof(test_data));

  zmq_msg_t       zmq_msg_out;
  int serialize_rc = socomm_serialize_message(msg_out, &zmq_msg_out);
  assert(serialize_rc == 0);
  assert(zmq_msg_size(&zmq_msg_out)
         >= (sizeof(socomm_header) + sizeof(uint64_t)));
  assert(memcmp((char *)socomm_message_data(msg_out),
                ((char *)zmq_msg_data(&zmq_msg_out) + 40),
                socomm_message_data_size(msg_out))
         == 0);

  socomm_message *msg_in = socomm_deserialize_message(&zmq_msg_out);
  zmq_msg_close(&zmq_msg_out);

  assert(msg_in != NULL);
  assert(socomm_message_data_size(msg_in) == socomm_message_data_size(msg_out));
  assert(socomm_message_data_size(msg_in) == data_size);

  const void *data_out = socomm_message_data(msg_out);
  const void *data_in  = socomm_message_data(msg_in);
  assert(memcmp(data_out, data_in, socomm_message_data_size(msg_out)) == 0);
  assert(memcmp((void *)test_data, data_in, socomm_message_data_size(msg_out))
         == 0);

  socomm_message_destroy(&msg_out);
  socomm_message_destroy(&msg_in);
}

int main()
{
  test_messages();
  test_serde();
  return 0;
}
