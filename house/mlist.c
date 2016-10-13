/* Smart Home: House server
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence 3
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#include "mlist.h"
#include <stdlib.h>


static struct meteo_list *meteo_list_last(struct meteo_list *restrict list)
{
    if (list)
        while (list->next)
            list = list->next;
    return list;
}


struct meteo_list *meteo_list_append(struct meteo_list *list, struct meteo *mdata)
{
    struct meteo_list *new_list;
    struct meteo_list *last;

    new_list = (struct meteo_list *)malloc(sizeof(new_list));
    new_list->mdata = mdata;
    new_list->next = NULL;

    if (list) {
        last = meteo_list_last(list);
        last->next = new_list;
        return list;
    }
    return new_list;
}

struct meteo *meteo_list_get_meteo(struct meteo_list *list)
{
    return list->mdata;
}

struct meteo_list *meteo_list_next(struct meteo_list *list)
{
    return list->next;
}

unsigned meteo_list_get_count(struct meteo_list *mlist)
{
    unsigned cnt = 0;

    for (struct meteo_list *l = mlist; l != NULL; l = meteo_list_next(l))
        cnt++;

    return cnt;
}

void meteo_list_free_all(struct meteo_list *list)
{
    struct meteo_list *tmp;

    if (!list)
        return;

    while (list) {
        tmp = list;
        list = list->next;
        if (tmp->mdata != NULL)
        free(tmp->mdata);
        free(tmp);
    }
}