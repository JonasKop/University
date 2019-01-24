#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "protocols.h"

int test_REG(void);

int test_ACK(void);

int test_ALIVE(void);

int test_NOTREG(void);

int test_GETLIST(void);

int test_SLIST(void);

int test_MESS(void);

int test_QUIT(void);

int test_JOIN(void);

int test_PJOIN(void);

int test_PLEAVE(void);

int test_PART(void);

int test_pdu(int op);

int calc_SLIST_ITEM_len(SLIST_ITEM *sitem);

int calc_SLIST_len(SLIST_msg slist);

int slist_itemcmp(SLIST_ITEM *sitem1, SLIST_ITEM *sitem2);

int slistcmp(SLIST_msg *slist1, SLIST_msg *slist2);


int main(void) {
    fprintf(stderr, "**************************************************\n");
    fprintf(stderr, "* %-46s *\n", "Testsuite for all PDUs.");
    fprintf(stderr, "**************************************************\n\n");
    int ret = test_pdu(REG);
    ret += test_pdu(ACK);
    ret += test_pdu(ALIVE);
    ret += test_pdu(NOTREG);
    ret += test_pdu(SLIST);
    ret += test_pdu(GETLIST);
    ret += test_pdu(MESS);
    ret += test_pdu(QUIT);
    ret += test_pdu(JOIN);
    ret += test_pdu(PJOIN);
    ret += test_pdu(PLEAVE);
    ret += test_pdu(PARTICIPANTS);

    fprintf(stderr, "\n**************************************************\n");
    if (ret == 12) {
        fprintf(stderr, "* %-46s *\n", "Congratulations, all tests were successful!");
    } else {
        char buf[64];
        sprintf(buf, "ERROR: %d PDU:s were incorrect", 12 - ret);
        fprintf(stderr, "* %-46s *\n", buf);
    }
    fprintf(stderr, "**************************************************\n");
    return 0;
}

int test_pdu(int op) {
    switch (op) {
        case REG:
            return test_REG();
        case ACK:
            return test_ACK();
        case ALIVE:
            return test_ALIVE();
        case NOTREG:
            return test_NOTREG();
        case GETLIST:
            return test_GETLIST();
        case SLIST:
            return test_SLIST();
        case MESS:
            return test_MESS();
        case QUIT:
            return test_QUIT();
        case JOIN:
            return test_JOIN();
        case PJOIN:
            return test_PJOIN();
        case PLEAVE:
            return test_PLEAVE();
        case PARTICIPANTS:
            return test_PART();
        default:
            return -1;
    }
}

int test_REG(void) {
    REG_msg *reg = malloc(sizeof(REG_msg));
    reg->len = 5;
    reg->port = 12345;

    reg->server_name = malloc((unsigned int) reg->len + 1);
    strcpy(reg->server_name, "fiska");

    pdu *p = malloc(sizeof(pdu));
    p->op = REG;
    p->msg = reg;

    uint8_t msg[BUFSIZE];
    memset(msg, 0, BUFSIZE);

    serialize_PDU(msg, p);
    int sock = 1, status, ind = 0;

    pdu *p2 = retrieve_msg(sock, msg, &status, &ind);

    REG_msg *reg2 = p2->msg;

    if ((reg2->port == reg->port) && (reg2->len == reg->len)
        && (strcmp(reg->server_name, reg2->server_name) == 0)) {
        fprintf(stderr, "Success: REG\n");
        destroy_PDU(p);
        destroy_PDU(p2);
        return 1;
    } else {
        fprintf(stderr, "Failed: REG\n");
        destroy_PDU(p);
        destroy_PDU(p2);
        return 0;
    }
}

int test_ACK(void) {
    ACK_msg *ack = malloc(sizeof(ACK_msg));
    ack->id = 93;

    pdu *p = malloc(sizeof(pdu));
    p->op = ACK;
    p->msg = ack;

    uint8_t msg[BUFSIZE];
    memset(msg, 0, BUFSIZE);

    serialize_PDU(msg, p);

    int sock = 1, status, ind = 0;
    pdu *p2 = retrieve_msg(sock, msg, &status, &ind);

    ACK_msg *ack2 = p2->msg;
    if (ack2->id == ack->id) {
        fprintf(stderr, "Success: ACK\n");
        destroy_PDU(p);
        destroy_PDU(p2);
        return 1;
    } else {
        fprintf(stderr, "Failed: ACK\n");
        destroy_PDU(p);
        destroy_PDU(p2);
        return 0;
    }
}


int test_ALIVE(void) {
    ALIVE_msg *alive = malloc(sizeof(ALIVE_msg));
    alive->id = 145;
    alive->nr_of_clients = 230;

    pdu *p = malloc(sizeof(pdu));
    p->op = ALIVE;
    p->msg = alive;

    uint8_t msg[BUFSIZE];
    memset(msg, 0, BUFSIZE);

    serialize_PDU(msg, p);

    int sock = 1, status, ind = 0;
    pdu *p2 = retrieve_msg(sock, msg, &status, &ind);

    ALIVE_msg *alive2 = p2->msg;
    if ((alive2->id == alive->id) && (alive2->nr_of_clients == alive->nr_of_clients)) {
        fprintf(stderr, "Success: ALIVE\n");
        destroy_PDU(p);
        destroy_PDU(p2);
        return 1;
    } else {
        fprintf(stderr, "Failed: ALIVE\n");
        destroy_PDU(p);
        destroy_PDU(p2);
        return 0;
    }
}

int test_NOTREG(void) {
    NOTREG_msg *notreg = malloc(sizeof(NOTREG_msg));
    notreg->id = 166;
    pdu *p = malloc(sizeof(pdu));
    p->op = NOTREG;
    p->msg = notreg;

    uint8_t msg[BUFSIZE];
    memset(msg, 0, BUFSIZE);

    serialize_PDU(msg, p);

    int sock = 1, status, ind = 0;
    pdu *p2 = retrieve_msg(sock, msg, &status, &ind);

    NOTREG_msg *notreg2 = p2->msg;
    if (notreg2->id == notreg->id) {
        fprintf(stderr, "Success: NOTREG\n");
        destroy_PDU(p);
        destroy_PDU(p2);
        return 1;
    } else {
        fprintf(stderr, "Failed: NOTREG\n");
        destroy_PDU(p);
        destroy_PDU(p2);
        return 0;
    }
}

int test_GETLIST(void) {
    GETLIST_msg *getlist = malloc(sizeof(GETLIST_msg));
    pdu *p = malloc(sizeof(pdu));
    p->op = GETLIST;
    p->msg = getlist;

    uint8_t msg[BUFSIZE];
    memset(msg, 0, BUFSIZE);

    serialize_PDU(msg, p);

    int sock = 1, status, ind = 0;
    pdu *p2 = retrieve_msg(sock, msg, &status, &ind);

    if (p2->op == p->op) {
        fprintf(stderr, "Success: GETLIST\n");
        destroy_PDU(p);
        destroy_PDU(p2);
        return 1;
    } else {
        fprintf(stderr, "Failed: GETLIST\n");
        destroy_PDU(p);
        destroy_PDU(p2);
        return 0;
    }
}


int calc_SLIST_ITEM_len(SLIST_ITEM *sitem) {
    return 8 + sitem->len + calc_pad(sitem->len);
}

int calc_SLIST_len(SLIST_msg slist) {
    int ret = 0;
    for (int i = 0; i < slist.nr_of_servers; ++i) {
        ret += calc_SLIST_ITEM_len(slist.server_info[i]);
    }
    return ret;
}

int slist_itemcmp(SLIST_ITEM *sitem1, SLIST_ITEM *sitem2) {
    int addr = 0;

    for (int i = 0; i < 4; ++i) {

        addr += sitem1->addr[i] == sitem2->addr[i];
    }
    return (addr == 4 && sitem1->len == sitem2->len
            && sitem1->port == sitem2->port && sitem1->nr_of_clients == sitem2->nr_of_clients
            && strcmp(sitem1->server_name, sitem2->server_name) == 0);
}

int slistcmp(SLIST_msg *slist1, SLIST_msg *slist2) {
    if (slist1->nr_of_servers != slist2->nr_of_servers) return 0;

    for (int i = 0; i < slist1->nr_of_servers; ++i) {
        if (slist_itemcmp(slist1->server_info[i], slist2->server_info[i]) == 0) return 0;
    }
    return 1;
}

int test_SLIST(void) {
    SLIST_ITEM *item1 = malloc(sizeof(SLIST_ITEM));
    item1->addr[0] = 192;
    item1->addr[1] = 168;
    item1->addr[2] = 1;
    item1->addr[3] = 1;
    item1->port = 1337;
    item1->len = 6;
    item1->nr_of_clients = 23;

    item1->server_name = malloc((unsigned int) item1->len + 1);
    strcpy(item1->server_name, "jannep");

    SLIST_ITEM *item2 = malloc(sizeof(SLIST_ITEM));
    item2->addr[0] = 193;
    item2->addr[1] = 169;
    item2->addr[2] = 2;
    item2->addr[3] = 2;
    item2->port = 1338;
    item2->len = 10;
    item2->nr_of_clients = 44;

    item2->server_name = malloc((unsigned int) item2->len + 1);
    strcpy(item2->server_name, "jannepanne");


    SLIST_ITEM **slist_items = malloc(sizeof(SLIST_ITEM *) * 2 + 1);
    slist_items[0] = item1;
    slist_items[1] = item2;

    SLIST_msg *slist = malloc(sizeof(SLIST_msg));
    slist->nr_of_servers = 2;
    slist->server_info = slist_items;

    pdu *p = malloc(sizeof(pdu));
    p->op = SLIST;
    p->msg = slist;

    uint8_t msg[BUFSIZE];
    memset(msg, 0, BUFSIZE);

    serialize_PDU(msg, p);
    int sock = 1, status, ind = 0;
    pdu *p2 = retrieve_msg(sock, msg, &status, &ind);
    SLIST_msg *slist2 = p2->msg;

    if (slistcmp(slist, slist2)) {
        fprintf(stderr, "Success: SLIST\n");
        destroy_PDU(p);
        destroy_PDU(p2);
        return 1;
    } else {
        fprintf(stderr, "Failed: SLIST\n");
        destroy_PDU(p);
        destroy_PDU(p2);
        return 0;
    }
}

int test_MESS(void) {
    MESS_msg *mess = malloc(sizeof(MESS_msg));
    mess->timestamp = 10;
    mess->id_len = 6;
    mess->msg_len = 12;
    mess->checksum = 20;
    mess->msg = malloc((unsigned int) mess->msg_len + 1);
    mess->client_id = malloc((unsigned int) mess->id_len + 1);
    strcpy(mess->msg, "tjosan berit");
    strcpy(mess->client_id, "jannep");
    pdu *p = malloc(sizeof(pdu));
    p->op = MESS;
    p->msg = mess;

    uint8_t msg[BUFSIZE];
    memset(msg, 0, BUFSIZE);

    serialize_PDU(msg, p);
    int sock = 1, status, ind = 0;
    pdu *p2 = retrieve_msg(sock, msg, &status, &ind);

    MESS_msg *mess2 = p2->msg;

    if ((mess2->msg_len == mess->msg_len) && (mess2->id_len == mess->id_len) &&
        mess2->checksum == mess->checksum && strcmp(mess2->client_id, mess->client_id) == 0
        && strcmp(mess2->msg, mess->msg) == 0 && mess2->timestamp == mess->timestamp) {
        fprintf(stderr, "Success: MESS\n");
        destroy_PDU(p);
        destroy_PDU(p2);
        return 1;
    } else {
        fprintf(stderr, "Failed: MESS\n");
        destroy_PDU(p);
        destroy_PDU(p2);
        return 0;
    }
}

int test_QUIT(void) {
    QUIT_msg quit = malloc(sizeof(QUIT_msg));
    pdu *p = malloc(sizeof(pdu));
    p->op = QUIT;
    p->msg = quit;

    uint8_t msg[BUFSIZE];
    memset(msg, 0, BUFSIZE);

    serialize_PDU(msg, p);

    int sock = 1, status, ind = 0;
    pdu *p2 = retrieve_msg(sock, msg, &status, &ind);


    if (p2->op == p->op) {
        fprintf(stderr, "Success: QUIT\n");
        destroy_PDU(p);
        destroy_PDU(p2);
        return 1;
    } else {
        fprintf(stderr, "Failed: QUIT\n");
        destroy_PDU(p);
        destroy_PDU(p2);
        return 0;
    }
}

int test_JOIN(void) {
    JOIN_msg *join = malloc(sizeof(JOIN_msg));
    join->len = 6;
    join->id = malloc((unsigned int) join->len + 1);
    strcpy(join->id, "jannep");

    pdu *p = malloc(sizeof(pdu));
    p->op = JOIN;
    p->msg = join;
    uint8_t msg[BUFSIZE];
    memset(msg, 0, BUFSIZE);

    serialize_PDU(msg, p);

    int sock = 1, status, ind = 0;
    pdu *p2 = retrieve_msg(sock, msg, &status, &ind);
    JOIN_msg *join2 = p2->msg;

    if ((join->len == join2->len) && (strcmp(join->id, join2->id) == 0)) {
        fprintf(stderr, "Success: JOIN\n");
        destroy_PDU(p);
        destroy_PDU(p2);
        return 1;
    } else {
        fprintf(stderr, "Failed: JOIN\n");
        destroy_PDU(p);
        destroy_PDU(p2);
        return 0;
    }
}

int test_PJOIN(void) {

    PJOIN_msg *pjoin = malloc(sizeof(PJOIN_msg));
    pjoin->len = 5;
    pjoin->timestamp = 33;

    pjoin->id = malloc((unsigned int) pjoin->len + 1);
    strcpy(pjoin->id, "janne");

    pdu *p = malloc(sizeof(pdu));
    p->op = PJOIN;
    p->msg = pjoin;

    uint8_t msg[BUFSIZE];
    memset(msg, 0, BUFSIZE);

    serialize_PDU(msg, p);

    int sock = 1, status, ind = 0;
    pdu *p2 = retrieve_msg(sock, msg, &status, &ind);

    PJOIN_msg *pjoin2 = p2->msg;
    if ((pjoin->len == pjoin2->len)
        && (strcmp(pjoin->id, pjoin2->id) == 0)
        && (pjoin->timestamp == pjoin2->timestamp)) {
        fprintf(stderr, "Success: PJOIN\n");
        destroy_PDU(p);
        destroy_PDU(p2);
        return 1;
    } else {
        fprintf(stderr, "Failed: PJOIN\n");
        destroy_PDU(p);
        destroy_PDU(p2);
        return 0;
    }
}

int test_PLEAVE(void) {
    PLEAVE_msg *pleave = malloc(sizeof(PLEAVE_msg));
    pleave->len = 5;
    pleave->timestamp = 33;

    pleave->id = malloc((unsigned int) pleave->len + 1);
    strcpy(pleave->id, "janne");

    pdu *p = malloc(sizeof(pdu));
    p->op = PLEAVE;
    p->msg = pleave;

    uint8_t msg[BUFSIZE];
    memset(msg, 0, BUFSIZE);

    serialize_PDU(msg, p);

    int sock = 1, status, ind = 0;
    pdu *p2 = retrieve_msg(sock, msg, &status, &ind);

    PJOIN_msg *pleave2 = p2->msg;
    if ((pleave->len == pleave2->len)
        && (strcmp(pleave->id, pleave2->id) == 0)
        && (pleave->timestamp == pleave2->timestamp)) {
        fprintf(stderr, "Success: PLEAVE\n");
        destroy_PDU(p);
        destroy_PDU(p2);
        return 1;
    } else {
        fprintf(stderr, "Failed: PLEAVE\n");
        destroy_PDU(p);
        destroy_PDU(p2);
        return 0;
    }
}

int test_PART(void) {
    PART_msg *part = malloc(sizeof(PART_msg));
    part->nr_of_ids = 3;
    part->ids_len = 14;

    part->ids = malloc(3 * sizeof(char *));
    part->ids[0] = malloc(5);
    part->ids[1] = malloc(3);
    part->ids[2] = malloc(6);


    memcpy(part->ids[0], "apan\0", 5);
    memcpy(part->ids[1], "se\0", 3);
    memcpy(part->ids[2], "bakar\0", 6);

    pdu *p = malloc(sizeof(pdu));
    p->op = PARTICIPANTS;
    p->msg = part;

    uint8_t msg[BUFSIZE];
    memset(msg, 0, BUFSIZE);
    serialize_PDU(msg, p);

    int sock = 1, status, ind = 0;
    pdu *p2 = retrieve_msg(sock, msg, &status, &ind);

    PART_msg *part2 = p2->msg;

    int equal = 1;
    for (int i = 0; i < part->nr_of_ids; ++i) {
        if (strcmp(part->ids[i], part2->ids[i]) != 0) {
            equal = 0;
            break;
        }
    }

    if (part2->ids_len == part->ids_len && part2->nr_of_ids == part->nr_of_ids && equal) {
        fprintf(stderr, "Success: PARTICIPANTS\n");
        destroy_PDU(p);
        destroy_PDU(p2);
        return 1;
    } else {
        destroy_PDU(p);
        destroy_PDU(p2);
        fprintf(stderr, "Failed: PARTICIPANTS\n");
        return 0;
    }
}

