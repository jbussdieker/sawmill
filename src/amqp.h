void *amqp_open(char *hostname, int port, char *user, char *password);
void *amqp_publish(void *conn, char *exchange, char *routingkey, char *message);
void *amqp_close(void *conn);

