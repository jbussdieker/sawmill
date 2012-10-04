#include <amqp.h>

void *amqp_open(char *hostname, int port, char *user, char *password) {
  amqp_connection_state_t conn;
  int sockfd;

  conn = amqp_new_connection();
  sockfd = amqp_open_socket(hostname, port);
  amqp_set_sockfd(conn, sockfd);
  amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, user, password);
  amqp_channel_open(conn, 1);
  amqp_get_rpc_reply(conn);
}

void *amqp_publish(amqp_connection_state_t conn, char *exchange, char *routingkey, char *message) {
  amqp_basic_properties_t props;
  props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
  props.content_type = amqp_cstring_bytes("text/plain");
  props.delivery_mode = 2; // persistent delivery mode
  amqp_basic_publish(conn,
	1,
	amqp_cstring_bytes(exchange),
	amqp_cstring_bytes(routingkey),
	0,
	0,
	&props,
	amqp_cstring_bytes(message)
  );
}

void *amqp_close(amqp_connection_state_t conn) {
  amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
  amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
  amqp_destroy_connection(conn);
}

