# 队列

## 队列的使用

提供两种使用方式：1）包含头文件queue.h和queue.c，使用make；2）使用动态链接库libqueue.so

代码中需使用宏定义自行定义元素类型(默认为int)，需要注意的是类型不能为void，例如:

#define queue_type double 

### 1、初始化

两种初始化方式：1）静态初始化；2）动态初始化

~~~c
//static initializer
queue_t queue = QUEUE_INITIALIZER;

//dynamic initializer
queue_t queue;
queue_init(&queue);
~~~

### 2、提供的函数

~~~c
int queue_push(queue_t *queue, queue_type *value);
int queue_pop(queue_t *queue, queue_type *task);
int queue_destory(queue_t *queue);
int queue_empty(queue_t *queue);
~~~

## license

Copyright 20190508 Peng Wen (https://github.com/expectmeeting)

Licensed under the Apache License, Version 2.0; you may not use this file except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.