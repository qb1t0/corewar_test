#include "asm.h"

//ToDO: NOT FUCKING WORKING
int cw_calllabel(char *s, t_cmnd *c, int i, int type) //1-T_DIR, 0-T_IND                                          //trying 2 search calling label, returning adress
{
    t_label *tmp;

    tmp = g_file->labl;
    while(tmp)                                                              //toDO: write catting last argument function here
    {
        if (!ft_strcmp(tmp->name, s))
        {
            c->i_arg[i] = (unsigned int)(tmp->start - c->start);
            if (!g_tab[c->cmd].id2 && type) //if T_DIR && !id2
            {
                c->i_arg[i] = ((c->i_arg[i] << 8) & 0xFF00FF00) | \
                ((c->i_arg[i] >> 8) & 0xFF00FF);
                c->i_arg[i] = SWAP_UI(c->i_arg[i]);
            }
            c->i_arg[i] = SWAP_USI(c->i_arg[i]);
            return (tmp->start - c->start);
        }
        tmp = tmp->next;
    }
    return (424242);                                                         //calling label wasn't founded error
}

int cw_check_label(int error)
{
    int i;
    int value;
    t_cmnd *tmp;

    tmp = g_file->cmnd;
    while (tmp)
    {
        i = 1;                                                               //11 01 10 == command(IND(CODE: 3), REG(CODE: 1), DIR(CODE: 2))
        while (i <= g_tab[tmp->cmd].arg && error != 424242)
        {
            value = tmp->cipher >> (g_tab[tmp->cmd].arg - i) * 2;
            value &= 3;
            if (value == 2 && IS_LBL(tmp->args[i - 1] [1]))                       //if T_DIR
                error = cw_calllabel(tmp->args[i - 1] + 2, tmp, i - 1, 1);
            else if (value == 3 && IS_LBL(tmp->args[i - 1][0]))                  //if T_IND
                error = cw_calllabel(tmp->args[i - 1] + 1, tmp, i - 1, 0);
            i++;

        }
        tmp = tmp->next;
        if (error == 424242)
            return (cw_e(21));
    }
    return (1);
}

/*
** Adding new label
*/

int cw_label(int start, int len)
{
    t_label *l;

    len = start;
    while (g_s[g_i] != LABEL_CHAR)
        g_i++;
    if (g_i == start || (g_i != 0 && g_s[g_i - 1] == DIRECT_CHAR))          //if %: => its command argument, call cw_cmd function
        return (g_i == start ? cw_e(14) : cw_cmd(0, -1, -1, 1));            //if g_i == start => means label is empty, invalid label name error
    while (len < g_i)                                                       //reading label name
        if (!ft_strchr(LABEL_CHARS, g_s[len++]))
            return (cw_e(15));                                              //invalid label name(!= LABELS CHARS)
    l = (t_label *)malloc(sizeof(t_label));                                 //malloc() for new operation list
    l->start = g_bytes;                                                     //starting byte
    l->line = g_file->lines;                                                //saving number of line in our struct
    l->name = ft_strsub(g_s + start, 0, (size_t)(len - start));             //saving label name
    l->next = g_file->labl;                                                 //adding from start new label 2 our label list
    g_file->labl ? g_file->labl->prev = l : NULL;                           //creating prev list
    g_file->labl = l;                                                       //redefine global head to the start of the list
    g_i++;
    while (SPACE(g_s[g_i]))                                               //skips spaces after label
        g_i++;
    return ((!g_s[g_i] || (IS_COM(g_s[g_i]))) ? 1 : cw_cmd(0, -1, -1, 0));  //if empty || comment after - skip
    //else call cw_cmd function
}