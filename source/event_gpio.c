/*
Copyright (c) 2013-2014 Ben Croston

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <pthread.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>

#include "event_gpio.h"
#include "c_gpio.h"
#include "common.h"

const char *stredge[4] = { "none", "rising", "falling", "both" };

struct gpios
{
    unsigned int gpio;
    int value_fd;
    int exported;
    int initial;
    unsigned int bouncetime;
    unsigned long long lastcall;
    struct gpios *next;
};
struct gpios *gpio_list = NULL;

// event callbacks
struct callback
{
    unsigned int gpio;
    void(*func)(unsigned int gpio);
    struct callback *next;
};
struct callback *callbacks = NULL;

pthread_t threads;
int event_occurred[64] = { 0 };
int thread_running = 0;
int epfd = -1;



/************* /sys/class/gpio functions ************/
int gpio_export(unsigned int gpio) {
    // unsigned int gpioSys = gpioToSysPin(gpio);
    int fd, len;
    char fName[64] = "/sys/class/gpio/export";

    #if NP_DEBUG
        printf("[%d] %s\n", gpio, fName);
    #endif

    if ( (fd = fopen(fName, "w")) == NULL ) {
        fprintf (stderr, "Unable to open GPIO export interface: %s\n", strerror(errno)) ;
        return -1;
    }
    fprintf(fd, "%d\n", gpio);
    fclose(fd);
    return 0;
}

int gpio_unexport(unsigned int gpio) {
    /* */
    int fd;
    char fName[64] = "/sys/class/gpio/unexport";
    /* */
    #if NP_DEBUG
        printf("[%d] %s\n", gpio, fName);
    #endif
    /* */
    if ((fd = fopen(fName, "w")) == NULL) {
        fprintf(stderr, "Unable to open GPIO export interface: %s\n", strerror(errno)) ;
        return -1;
    }
    fprintf(fd, "%d\n", gpio);
    fclose(fd);
    /* */
    return 0;
}

int gpio_set_direction(unsigned int gpio, unsigned int in_flag) {
    // unsigned int gpioSys = gpioToSysPin(gpio);
    int fd;
    char fName[128] = {0};

    sprintf(fName, "/sys/class/gpio/gpio%d/direction", gpio);
    #if NP_DEBUG
        printf("%s\n", fName);
    #endif
    if ((fd = fopen (fName, "w")) == NULL) {
        fprintf (stderr, "Unable to open GPIO direction interface for gpio %d: %s\n", gpio, strerror(errno)) ;
        return -1;
    }

    if (in_flag)
        write(fd, "in", 3);
    else
        write(fd, "out", 4);

    close(fd);
    return 0;
}

int gpio_set_edge(unsigned int gpio, unsigned int edge)
{
    // unsigned int gpioSys = gpioToSysPin(gpio);
    int fd;
    char filename[64];

    snprintf(filename, sizeof(filename), "/sys/class/gpio/gpio%d/edge", gpio);

    #if NP_DEBUG
        printf("/sys/class/gpio/gpio%d/edge\n", gpio);
    #endif

    if ((fd = open(filename, O_WRONLY)) < 0)
        return -1;

    write(fd, stredge[edge], strlen(stredge[edge]) + 1);
    close(fd);
    return 0;
}

int gpio_check(unsigned int gpio)
{
    // unsigned int gpioSys = gpioToSysPin(gpio);
    int fd;
    char filename[64];

    snprintf(filename, sizeof(filename), "/sys/class/gpio/gpio%d", gpio);
    if ((fd = open(filename, O_RDONLY)) < 0)
        return -1;

    close(fd);
    return 0;
}

int gpio_set_value(unsigned int gpio, unsigned int value)
{
    // unsigned int gpioSys = gpioToSysPin(gpio);
    int fd;
    char filename[64];

    snprintf(filename, sizeof(filename), "/sys/class/gpio/gpio%d/value", gpio);
    if ((fd = open(filename, O_WRONLY)) < 0)
        return -1;

    if (value)
        write(fd, "1", 2);
    else
        write(fd, "0", 2);

    close(fd);
    return 0;
}

int gpio_get_value(unsigned int gpio)
{
    // unsigned int gpioSys = gpioToSysPin(gpio);
    int fd;
    char filename[64];
    char buf;

    snprintf(filename, sizeof(filename), "/sys/class/gpio/gpio%d/value", gpio);
    if ((fd = open(filename, O_RDONLY)) < 0)
        return -1;

    if (read(fd, &buf, 1) != 1)
        return -1;

#if NP_DEBUG
    printf("NPAPI: __%d__(%s:%s) value = %c, %d\n", __LINE__, __FUNCTION__, __FILE__, buf, atoi(&buf));
#endif

    close(fd);
    return atoi(&buf);
}

int gpio_set_pull(unsigned int gpio, unsigned int value)
{
    // unsigned int gpioSys = gpioToSysPin(gpio);
    int fd;
    char filename[64];

    snprintf(filename, sizeof(filename), "/sys/class/gpio/gpio%d/pull", gpio);
    if ((fd = open(filename, O_WRONLY)) < 0)
        return -1;

    if (value == PUD_DOWN) //pull down
        write(fd, "down", 5);
    else if (value == PUD_UP) //pull up
        write(fd, "up", 3);
    else //default == PUD_OFF, pull disable
        write(fd, "disable", 8);

    close(fd);
    return 0;
}

int open_value_file(unsigned int gpio)
{
    // unsigned int gpioSys = gpioToSysPin(gpio);
    int fd;
    char fName[64];

    sprintf(fName, "/sys/class/gpio/gpio%d/value", gpio);
    if ( (fd = open(fName, O_RDONLY)) == -1 ) {
        printf("No Value file (huh?)\n");
        return -1;
    }

    // create file descriptor of value file
    // snprintf(filename, sizeof(filename), "/sys/class/gpio/gpio%d/value", gpio);
    // if ((fd = open(filename, O_RDONLY | O_NONBLOCK)) < 0)
    //     return -1;
    return fd;
}



/********* gpio list functions **********/
struct gpios *get_gpio(unsigned int gpio)
{
    struct gpios *g = gpio_list;
    while (g != NULL) {
        if (g->gpio == gpio)
            return g;
        g = g->next;
    }
    return NULL;
}

struct gpios *get_gpio_from_value_fd(int fd)
{
    struct gpios *g = gpio_list;
    while (g != NULL) {
        if (g->value_fd == fd)
            return g;
        g = g->next;
    }
    return NULL;
}

struct gpios *new_gpio(unsigned int gpio)
{
    struct gpios *new_gpio;

    new_gpio = malloc(sizeof(struct gpios));

    if (new_gpio == 0)
        return NULL;  // out of memory

    new_gpio->gpio = gpio;
    if (gpio_export(gpio) != 0) {
        free(new_gpio);
        printf("gpio_export error\n");
        return NULL;
    }
    new_gpio->exported = 1;

    if (gpio_set_direction(gpio, 1) != 0) { // 1==input
        free(new_gpio);
        printf("gpio_set_direction error\n");
        return NULL;
    }

    if ((new_gpio->value_fd = open_value_file(gpio)) == -1) {
        gpio_unexport(gpio);
        free(new_gpio);
        printf("open_value_file error\n");
        return NULL;
    }

    new_gpio->initial = 1;
    new_gpio->bouncetime = 0;
    new_gpio->lastcall = 0;

    if (gpio_list == NULL) {
        new_gpio->next = NULL;
    }
    else {
        new_gpio->next = gpio_list;
    }
    gpio_list = new_gpio;
    return new_gpio;
}

void delete_gpio(unsigned int gpio)
{
    struct gpios *g = gpio_list;
    struct gpios *temp;
    struct gpios *prev = NULL;

    while (g != NULL) {
        if (g->gpio == gpio) {
            if (prev == NULL)
                gpio_list = g->next;
            else
                prev->next = g->next;
            temp = g;
            g = g->next;
            free(temp);
        }
        else {
            prev = g;
            g = g->next;
        }
    }
}



/******* callback list functions ********/
int add_edge_callback(unsigned int gpio, void(*func)(unsigned int gpio))
{
    struct callback *cb = callbacks;
    struct callback *new_cb;

    new_cb = malloc(sizeof(struct callback));
    if (new_cb == 0)
        return -1;  // out of memory

    new_cb->gpio = gpio;
    new_cb->func = func;
    new_cb->next = NULL;

    if (callbacks == NULL) {
        // start new list
        callbacks = new_cb;
    }
    else {
        // add to end of list
        while (cb->next != NULL)
            cb = cb->next;
        cb->next = new_cb;
    }
    return 0;
}

void run_callbacks(unsigned int gpio)
{
    struct callback *cb = callbacks;
    while (cb != NULL)
    {
        if (cb->gpio == gpio)
            cb->func(cb->gpio);
        cb = cb->next;
    }
}

void remove_callbacks(unsigned int gpio)
{
    struct callback *cb = callbacks;
    struct callback *temp;
    struct callback *prev = NULL;

    while (cb != NULL)
    {
        if (cb->gpio == gpio)
        {
            if (prev == NULL)
                callbacks = cb->next;
            else
                prev->next = cb->next;
            temp = cb;
            cb = cb->next;
            free(temp);
        }
        else {
            prev = cb;
            cb = cb->next;
        }
    }
}

void *poll_thread(void *threadarg)
{
    struct epoll_event events;
    char buf;
    struct timeval tv_timenow;
    unsigned long long timenow;
    struct gpios *g;
    int n;

    thread_running = 1;
    while (thread_running) {
        if ((n = epoll_wait(epfd, &events, 1, -1)) == -1) {
            thread_running = 0;
            pthread_exit(NULL);
        }
        if (n > 0) {
            lseek(events.data.fd, 0, SEEK_SET);
            if (read(events.data.fd, &buf, 1) != 1) {
                thread_running = 0;
                pthread_exit(NULL);
            }
            g = get_gpio_from_value_fd(events.data.fd);
            if (g->initial) {     // ignore first epoll trigger
                g->initial = 0;
            }
            else {
                gettimeofday(&tv_timenow, NULL);
                timenow = tv_timenow.tv_sec*1E6 + tv_timenow.tv_usec;
                if (g->bouncetime == 0 || timenow - g->lastcall > g->bouncetime * 1000 || g->lastcall == 0 || g->lastcall > timenow) {
                    g->lastcall = timenow;
                    event_occurred[g->gpio] = 1;
                    run_callbacks(g->gpio);
                }
            }
        }
    }
    thread_running = 0;
    pthread_exit(NULL);
}

void remove_edge_detect(unsigned int gpio)
{
    struct epoll_event ev;
    struct gpios *g = get_gpio(gpio);

    if (g == NULL)
        return;

    // delete epoll of fd
    epoll_ctl(epfd, EPOLL_CTL_DEL, g->value_fd, &ev);

    // delete callbacks for gpio
    remove_callbacks(gpio);

    // btc fixme - check return result??
    gpio_set_edge(gpio, NO_EDGE);

    if (g->value_fd != -1)
        close(g->value_fd);

    // btc fixme - check return result??
    gpio_unexport(gpio);
    event_occurred[gpio] = 0;

    delete_gpio(gpio);
}

int event_detected(unsigned int gpio)
{
    if (event_occurred[gpio]) {
        event_occurred[gpio] = 0;
        return 1;
    }
    else {
        return 0;
    }
}

void event_cleanup(unsigned int gpio)
// gpio of -666 means clean every channel used
{
    while (gpio_list != NULL) {
        if ( (gpio == (unsigned int)-666) || (gpio_list->gpio == gpio) )
            remove_edge_detect(gpio_list->gpio);
    }
    thread_running = 0;
}

void event_cleanup_all(void)
{
    event_cleanup(-666);
}

int gpio_event_added(unsigned int gpio)
{
    struct gpios *g = gpio_list;
    while (g != NULL) {
        if (g->gpio == gpio)
            return 1;
        g = g->next;
    }
    return 0;
}

int add_edge_detect(unsigned int gpio, unsigned int edge, unsigned int bouncetime) {
    pthread_t threads;
    struct epoll_event ev;
    long t = 0;
    struct gpios *g;

    // check to see if this gpio has been added already
    if (gpio_event_added(gpio) != 0)
        return 1;

    // create epfd if not already open
    if ((epfd == -1) && ((epfd = epoll_create(1)) == -1))
        return 2;

    if ((g = new_gpio(gpio)) == NULL) {
        return 3;
    }

    gpio_set_edge(gpio, edge);
    g->bouncetime = bouncetime;

    // add to epoll fd
    ev.events = EPOLLIN | EPOLLET | EPOLLPRI;
    ev.data.fd = g->value_fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, g->value_fd, &ev) == -1) {
        remove_edge_detect(gpio);
        return 4;
    }

    // start poll thread if it is not already running
    if (!thread_running) {
        if (pthread_create(&threads, NULL, poll_thread, (void *)t) != 0) {
            remove_edge_detect(gpio);
            return 5;
        }
    }
    return 0;
}

int blocking_wait_for_edge(unsigned int gpio, unsigned int edge)
// standalone from all the event functions above
{
    int epfd, fd, n, i;
    struct epoll_event events, ev;
    char buf;

    // check to see if this gpio has been added already
    if (gpio_event_added(gpio))
        return 1;

    if ((epfd = epoll_create(1)) == -1)
        return 2;

    // export /sys/class/gpio interface
    gpio_export(gpio);
    gpio_set_direction(gpio, 1); // 1=input
    gpio_set_edge(gpio, edge);
    if ((fd = open_value_file(gpio)) == -1)
        return 3;

    // add to epoll fd
    ev.events = EPOLLIN | EPOLLET | EPOLLPRI;
    ev.data.fd = fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        close(fd);
        gpio_unexport(gpio);
        return 4;
    }

    // epoll for event
    for (i = 0; i < 2; i++) // first time triggers with current state, so ignore
        if ((n = epoll_wait(epfd, &events, 1, -1)) == -1) {
            epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev);
            close(fd);
            gpio_unexport(gpio);
            return 5;
        }

    if (n > 0) {
        lseek(events.data.fd, 0, SEEK_SET);
        if (read(events.data.fd, &buf, 1) != 1) {
            epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev);
            close(fd);
            gpio_unexport(gpio);
            close(epfd);
            return 6;
        }
        if (events.data.fd != fd) {
            epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev);
            close(fd);
            gpio_unexport(gpio);
            return 7;
        }
    }

    // clean up
    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev);
    close(fd);
    gpio_unexport(gpio);
    return 0;
}
