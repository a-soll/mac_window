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
    snprintf(buf + strlen(buf), buffsize, "wid=%d\n", window->wid);
    buf += snprintf(buf + strlen(buf), buffsize, "space=%llu\n", current_space_for_window(window));
    buf += snprintf(buf + strlen(buf), buffsize, "pos.x=%f\n", window->position.x);
    buf += snprintf(buf + strlen(buf), buffsize, "pos.y=%f\n", window->position.y);
    buf += snprintf(buf + strlen(buf), buffsize, "size.width=%f\n", window->size.width);
    buf += snprintf(buf + strlen(buf), buffsize, "size.height=%f\n\n", window->size.height);
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

static int app_count(char *buf) {
    int i = 0;
    int count = 0;

    while (buf[i]) {
        if (buf[i] == '[') {
            count++;
        }
        i++;
    }
    return count;
}

static int parse_app(char *buf, int index, char *app) {
    int new_ind = index + 1;
    int app_ind = 0;

    while (buf[new_ind] != ']') {
        app[app_ind] = buf[new_ind];
        app_ind++;
        new_ind++;
    }
    app[app_ind] = '\0';
    new_ind++;
    return new_ind;
}

// vscode inserts SOH (aka 1) at EOF
bool is_valid_token(char c) {
    if (c != '\0' && c != '\n' && c != 1) {
        return true;
    }
    return false;
}

int parse_val(char *buf, int ind, char *val) {
    int new_ind = ind + 1;
    int val_ind = 0;

    while (is_valid_token(buf[new_ind])) {
        val[val_ind] = buf[new_ind];
        val_ind++;
        new_ind++;
    }
    val[val_ind] = '\0';
    return new_ind;
}

int parse_key_name(char *buf, int ind, char *key) {
    int new_ind = ind;
    int i = 0;

    while (is_valid_token(buf[new_ind]) && buf[new_ind] != '=') {
        key[i] = buf[new_ind];
        i++;
        new_ind++;
    }
    key[i] = '\0';
    return new_ind;
}

static void to_double_maybe(char *key, char *val, char *attr, double *app_val) {
    if (strcmp(key, attr) == 0 && app_val > 0) {
        *app_val = atof(val);
    }
}

static void to_int_maybe(char *key, char *val, char *attr, int *app_val) {
    if (strcmp(key, attr) == 0 && app_val > 0) {
        *app_val = atoi(val);
    }
}

int load_config(INI **ini) {
    INI *tmp;
    const char *file = "config.ini";
    int fd = open(file, O_RDWR);
    struct stat info;

    fstat(fd, &info);
    char buf[info.st_size];

    if (read(fd, buf, info.st_size) < 0) {
        printf("couldn't read config\n");
        exit(1);
    }

    int num_apps = app_count(buf);
    tmp = malloc(sizeof(INI) * num_apps);
    int i = 0;
    int j = 0;
    while (buf[i] != '\0') {
        INI app;
        int key_len = 256;
        char path[PATH_MAX];
        char token = buf[i];
        char key[key_len];
        char val[key_len];
        if (token == '[') {
            i = parse_app(buf, i, path);
            memcpy(app.path, path, PATH_MAX);
            j++;
        }
        if (is_valid_token(buf[i])) {
            i = parse_key_name(buf, i, key);
            i = parse_val(buf, i, val);
            to_int_maybe(key, val, "wid", &app.wid);
            to_int_maybe(key, val, "space", &app.space);
            to_double_maybe(key, val, "pos.x", &app.position.x);
            to_double_maybe(key, val, "pos.y", &app.position.y);
            to_double_maybe(key, val, "size.width", &app.size.width);
            to_double_maybe(key, val, "size.height", &app.size.height);
            tmp[j - 1] = app;
        }
        i++;
    }
    *ini = tmp;
    return num_apps;
}
