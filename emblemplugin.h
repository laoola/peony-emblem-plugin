/*
 * @Author: 张文昊 
 * @Date: 2025-12-03 10:36:47
 * @LastEditors: 张文昊 
 * @LastEditTime: 2025-12-03 17:27:43
 * @FilePath: /linuxoverlay/peony-emblem-plugin/emblemplugin.h
 * @Description: 
 * 
 * Copyright (c) 2025  All Rights Reserved. 
 */
#ifndef EMBLEMPLUGIN_H
#define EMBLEMPLUGIN_H

#include <QObject>
#include <emblem-plugin-iface.h>
#include <emblem-provider.h>
#include "emblem_global.h"

#define SECRET_GONGKAI_ICON "secretGongkai"
#define SECRET_NEIBU_ICON "secretNeibu"
#define SECRET_MIMI_ICON "secretMimi"
#define SECRET_JIMI_ICON "secretJimi"
#define SECRET_JUEMI_ICON "secretJuemi"
#define SECRET_PUTONG_ICON "secretPutong"
#define SECRET_HEXIN_ICON "secretHexin"
#define SECRET_FEIMI_ICON "secretFeimi"


// 自定义的EmblemProvider实现类
class BadgeEmblemProvider : public Peony::EmblemProvider
{
    Q_OBJECT
public:
    explicit BadgeEmblemProvider(QObject *parent = nullptr);
    ~BadgeEmblemProvider() override;
    
    // 实现EmblemProvider接口方法
    const QString emblemKey() override;
    QStringList getFileEmblemIcons(const QString &uri) override;
    
private:
    // 辅助函数
    QString extractFilenameFromUri(const QString &uri);
    void initEmblemRules();
    
    // 新的辅助函数：根据文件名确定安全级别
    int determineSecurityLevel(const QString &filename);
    // 获取对应级别的图标路径
    QString getIconPathForLevel(int level);
    
    // 图标文件名列表
    QVector<QString> m_vtIcon;
    // 安全级别定义
    struct SecurityLevel {
        int level;         // 级别编号
        QString keyword;   // 中文关键词
        QString enKeyword; // 英文关键词
    };
    QVector<SecurityLevel> m_securityLevels;
};

// 插件接口实现类
class EMBLEMSHARED_EXPORT emblemPlugin : public QObject, public Peony::EmblemPluginInterface
{
    Q_OBJECT
    // 注册为Peony插件，使用正确的IID
    Q_PLUGIN_METADATA(IID EmblemPluginInterface_iid FILE "emblem_plugin.json")
    // 声明实现的接口
    Q_INTERFACES(Peony::EmblemPluginInterface)
    
public:
    explicit emblemPlugin(QObject *parent = nullptr);
    ~emblemPlugin() override;
    
    // 实现EmblemPluginInterface的纯虚函数
    Peony::EmblemProvider *create() override;
    
    // 实现PluginInterface的纯虚函数
    Peony::PluginInterface::PluginType pluginType() override;
    const QString name() override;
    const QString description() override;
    const QIcon icon() override;
    void setEnable(bool enable) override;
    bool isEnable() override;
    
private:
    bool m_enabled; // 插件启用状态
    BadgeEmblemProvider *m_provider; // 添加这个成员变量保存provider实例
};

#endif // EMBLEMPLUGIN_H