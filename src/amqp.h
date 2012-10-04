void *amqp_open(char *hostname, int port, char *user, char *password);
void *amqp_publish(amqp_connection_state_t conn, char *exchange, char *routingkey, char *message);
void *amqp_close(amqp_connection_state_t conn);

