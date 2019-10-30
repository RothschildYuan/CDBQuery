#ifndef CDBCONFIG_H
#define CDBCONFIG_H

#include <stdio.h>

#include "configure.h"

/*
 * 单例模式
 * 简单工厂模式
 * 抽象工厂模式
 * 模板方法模式
 * 代理模式
 * 外观模式
 * 适配器模式
 *
 */

/* xuwen——web
 * 观察者模式
 *
 */

class ACT_CPP_DLL_API QText
{
public:
    QText() {
        m_data  = nullptr;
        m_len   = 0;
    }
    QText(char* data, size_t len) {
        set(data, len);
    }
    void set(char* data, size_t len) {
        m_data  = data;
        m_len   = len;
    }

    char*   m_data;
    size_t  m_len;
};

#endif // CDBCONFIG_H
