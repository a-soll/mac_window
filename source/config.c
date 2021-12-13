#include "include/config.h"

extern Table *proc_table;
extern Table *app_table;
extern Table *window_table;

void write_file(char *buffer) {
    int config_fd = open("config.ini", O_CREAT | O_RDWR | O_TRUNC);
    if (config_fd == -1) {
        printf("Could not create config\n");
        exit(1);
    }
    write(config_fd, buffer, strlen(buffer));
}

static void config_str(Window *window, char *buf, int buffsize) {
    char *tmp = *(&buf);
    snprintf(tmp + strlen(tmp), buffsize, "wid=%d\n", window->wid);
    tmp += snprintf(tmp + strlen(tmp), buffsize, "space=%llu\n", current_space_for_window(window));
    tmp += snprintf(tmp + strlen(tmp), buffsize, "pos.x=%f\n", window->position.x);
    tmp += snprintf(tmp + strlen(tmp), buffsize, "pos.y=%f\n", window->position.y);
    tmp += snprintf(tmp + strlen(tmp), buffsize, "size.width=%f\n", window->size.width);
    tmp += snprintf(tmp + strlen(tmp), buffsize, "size.height=%f\n\n", window->size.height);
}

void snapshot() {
    int buffsize = 4096;
    char buf[buffsize];
    Application *cur_app;

    cur_app = (Application *)table_iterate(app_table, true);
    do {
        if (cur_app->windowCount > 0) {
            snprintf(buf + strlen(buf), buffsize, "[%s]\n", cur_app->path);
            for (int i = 0; i < cur_app->windowCount; i++) {
                Window *w = get_window(cur_app->wids[i]);
                config_str(w, buf, buffsize);
            }
        }
    } while ((cur_app = (Application *)table_iterate(app_table, false)));
    write_file(buf);
}

static int resize(char **c, int size) {
    int new_size = size * 2;
    *c = realloc(*c, sizeof(char *) * new_size);
    return new_size;
}

static int parse_app(char *buf, int index, char **app) {
    int new_ind = index + 1;
    int size = 25;
    int cur_ind = 0;
    *app = malloc(sizeof(char *) * size);
    char *tmp = *app;
    tmp[size - 1] = '\0';

    while (buf[new_ind] != ']') {
        new_ind++;
        if (tmp[cur_ind] == '\0') {
            size = resize(&tmp, size);
            tmp[size - 1] = '\0';
        }
        tmp[cur_ind] = buf[new_ind];
    }
    *app = tmp;
    new_ind++;
    return new_ind;
}

void load_config() {
    const char *file = "config.ini";
    int fd = open(file, O_RDWR);
    struct stat info;

    fstat(fd, &info);
    char buf[info.st_size];

    if (read(fd, buf, info.st_size) < 0) {
        printf("couldn't read config\n");
        exit(1);
    }

    char *app;
    int i = 0;
    while (buf[i] != '\0') {
        char token = buf[i];
        if (token == '[') {
            i = parse_app(buf, i, &app);
            printf("%s\n", app);
        }
        i++;
    }
}
