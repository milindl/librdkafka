/*
 * librdkafka - Apache Kafka C library
 *
 * Copyright (c) 2022, Magnus Edenhill
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * Example utility that shows how to use ListConsumerGroups (AdminAPI)
 */

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>


/* Typical include path would be <librdkafka/rdkafka.h>, but this program
 * is builtin from within the librdkafka source tree and thus differs. */
#include "rdkafka.h"


int main(int argc, char **argv) {
        rd_kafka_conf_t *conf; /* Temporary configuration object */
        char errstr[512];      /* librdkafka API error reporting buffer */
        const char *bootstrap_servers;    /* Argument: bootstrap servers */
        rd_kafka_t *rk;                   /* Admin client instance */

        bootstrap_servers = argv[1];

        conf = rd_kafka_conf_new();

        if (rd_kafka_conf_set(conf, "bootstrap.servers", bootstrap_servers,
                              errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
                fprintf(stderr, "%s\n", errstr);
                return 1;
        }

        rk = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr));
        if (!rk) {
                fprintf(stderr, "%% Failed to create new producer: %s\n",
                        errstr);
                return 1;
        }

        const struct rd_kafka_group_list *list;
        rd_kafka_resp_err_t e = rd_kafka_list_consumer_groups(rk, &list, NULL);
        if (e != RD_KAFKA_RESP_ERR_NO_ERROR) {
                printf("%s\n", rd_kafka_err2str(e));
                return 1;
        }
        for (int i = 0; i < list->group_cnt; i++) {
                struct rd_kafka_group_info gi = list->groups[i];
                printf("%s\n", gi.group);
        }

        /* Destroy the producer instance */
        rd_kafka_destroy(rk);

        return 0;
}
