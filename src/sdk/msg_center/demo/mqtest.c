/*******************************************************************************
 * Copyright (c) 2012, 2016 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution. 
 *
 * The Eclipse Public License is available at 
 *   http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at 
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial contribution
 *    Ian Craggs - change delimiter option from char to string
 *    Al Stockdill-Mander - Version using the embedded C client
 *    Ian Craggs - update MQTTClient function names
 *******************************************************************************/
#include "MQTTClient.h"
#include <signal.h>
#include <sys/time.h>

volatile int toStop = 0;

static void _finish(int sig)
{
  signal(SIGINT, NULL);
  toStop = 1;
}

struct opts_struct
{
  char      *clientid;
  int       nodelimiter;
  char      *delimiter;
  enum QoS  qos;
  char      *username;
  char      *password;
  char      *host;
  int       port;
  int       showtopics;
} opts =
{
  (char *)"f-SasnAd4fv9G6iRMuHNT6", 0, (char*)"\n", QOS0, "dac6cd7ca9ad49578db54c5db678b610", "2ac5b90adef74da3b47dff2204b7a67c",
  //(char *)"f-SasnAd4fv9G6iRMuHNT6", 0, (char*)"\n", QOS0, "1", "1",
  (char*)"msgsh.hivoice.cn", 1883, 0
};

void messageArrived(MessageData* md)
{
  MQTTMessage* message = md->message;

  if (opts.showtopics)
    printf("%.*s\t", md->topicName->lenstring.len, md->topicName->lenstring.data);
  if (opts.nodelimiter)
    printf("%.*s", (int)message->payloadlen, (char*)message->payload);
  else
    printf("%.*s%s", (int)message->payloadlen, (char*)message->payload, opts.delimiter);
}

int main(int argc, char** argv)
{
  int rc = 0;
  unsigned char buf[1000];
  unsigned char readbuf[1000];
  Network n;
  MQTTClient c;

  //char* topic = argv[1];
  char *topic = "s2c/msg/f-SasnAd4fv9G6iRMuHNT6";
  if (opts.showtopics) {
    printf("topic is %s\n", topic);
  }

  signal(SIGINT, _finish);
  signal(SIGTERM, _finish);

  NetworkInit(&n);
  NetworkConnect(&n, opts.host, opts.port);
  MQTTClientInit(&c, &n, 1000, buf, 1000, readbuf, 1000);

  MQTTPacket_connectData data = MQTTPacket_connectData_initializer;       

  //data.willFlag = 0;
  //data.MQTTVersion = 3;
  data.clientID.cstring = opts.clientid;
  data.username.cstring = opts.username;
  data.password.cstring = opts.password;
  

  data.keepAliveInterval = 100;
  data.cleansession = 1;
  printf("client_id: %s username %s password %s\n",
      data.clientID.cstring, data.username.cstring, data.password.cstring);
  printf("Connecting to %s %d\n", opts.host, opts.port);
  rc = MQTTConnect(&c, &data);
  printf("Connected %d\n", rc);
  printf("Subscribing to %s\n", topic);
  rc = MQTTSubscribe(&c, topic, opts.qos, messageArrived);
  printf("Subscribed %d\n", rc);
  //MQTTPublish(&c, "c2s/msg/f-SasnAd4fv9G6iRMuHNT6", "hello world");

  while (!toStop)
  {
    MQTTYield(&c, 1000);	
  }

  printf("Stopping\n");
  MQTTDisconnect(&c);
  NetworkDisconnect(&n);
  return 0;
}
