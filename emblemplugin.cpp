#include "emblemplugin.h"
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <iostream>

using namespace std;

// BadgeEmblemProvider实现
BadgeEmblemProvider::BadgeEmblemProvider(QObject *parent) : Peony::EmblemProvider(parent)
{
    qDebug() << "BadgeEmblemProvider初始化 - test start";
    initEmblemRules();
    qDebug() << "BadgeEmblemProvider初始化完成，规则数量:" << m_securityLevels.size();
}

BadgeEmblemProvider::~BadgeEmblemProvider()
{
}

const QString BadgeEmblemProvider::emblemKey()
{
    return "BadgeEmblemProvider";
}

QStringList BadgeEmblemProvider::getFileEmblemIcons(const QString &uri)
{
    qDebug() << "自定义角标插件: 处理文件:" << uri;
    QStringList emblems;
    
    // 检查是否为文件夹
    QUrl url(uri);
    if (url.isLocalFile()) {
        QFileInfo fileInfo(url.toLocalFile());
        if (fileInfo.isDir()) {
            qDebug() << "自定义角标插件: 跳过文件夹:" << uri;
            return emblems;  // 返回空列表，不处理文件夹
        }
    }

    // 提取文件名
    QString filename = extractFilenameFromUri(uri);
    qDebug() << "filename:" << filename;
    
    // 确定安全级别
    int level = determineSecurityLevel(filename);
    qDebug() << "文件安全级别:" << level;
    
    // 获取对应图标的路径
    QString iconPath = getIconPathForLevel(level);
    if (!iconPath.isEmpty()) {
        qDebug() << "自定义角标插件: 找到角标图标:" << iconPath;
        emblems << iconPath;
    }
    
    return emblems;
}

void BadgeEmblemProvider::initEmblemRules()
{
// 初始化安全级别规则（按照用户提供的规则）
    m_securityLevels.append({0, "公开", "gongkai"});
    m_securityLevels.append({1, "普通", "putong"});
    m_securityLevels.append({2, "内部", "neibu"});
    m_securityLevels.append({3, "秘密", "mimi"});
    m_securityLevels.append({4, "机密", "jimi"});
    m_securityLevels.append({5, "绝密", "juemi"});
    m_securityLevels.append({6, "核心", "hexin"});
    m_securityLevels.append({7, "非密", "feimi"});
    
    // 初始化图标文件名列表
    m_vtIcon.push_back("secretGongkai.ico");
    m_vtIcon.push_back("secretPutong.ico");
    m_vtIcon.push_back("secretNeibu.ico");
    m_vtIcon.push_back("secretMimi.ico");
    m_vtIcon.push_back("secretJimi.ico");
    m_vtIcon.push_back("secretJuemi.ico");
    m_vtIcon.push_back("secretHexin.ico");
    m_vtIcon.push_back("secretFeimi.ico");
}

QString BadgeEmblemProvider::extractFilenameFromUri(const QString &uri)
{
    QUrl url(uri);
    if (url.isLocalFile()) {
        return url.fileName();
    }
    return uri.split("/").last();
}

int BadgeEmblemProvider::determineSecurityLevel(const QString &filename)
{
    std::string stdFilename = filename.toStdString();
    
    // 遍历所有安全级别规则进行匹配
    for (const auto &level : m_securityLevels) {
        if (stdFilename.find(level.keyword.toStdString()) != std::string::npos ||
            stdFilename.find(level.enKeyword.toStdString()) != std::string::npos) {
            return level.level;
        }
    }
    
    // 默认返回公开级别
    return 0;
}

QString BadgeEmblemProvider::getIconPathForLevel(int level)
{
    // 检查级别是否有效
    if (level < 0 || level >= m_vtIcon.size()) {
        qDebug() << "警告: 无效的安全级别:" << level;
        return QString();
    }
    
    // 获取图标文件名
    QString iconName = m_vtIcon[level];
    
    // 构建资源路径
    QString resourcePath = ":/emblems/res/" + iconName;
    
    // 检查资源文件是否存在
    QFile resourceFile(resourcePath);
    if (resourceFile.exists()) {
        return resourcePath;
    }
    
    // 如果资源文件不存在，尝试本地路径
    QString localPath = "/home/zwh/Desktop/res/" + iconName;
    QFile localFile(localPath);
    if (localFile.exists()) {
        return localPath;
    }
    
    // 如果都不存在，记录错误
    qDebug() << "警告: 图标文件不存在 - 资源路径:" << resourcePath 
         << " 本地路径:" << localPath;
    
    return QString();
}

// emblemPlugin实现
emblemPlugin::emblemPlugin(QObject *parent) : QObject(parent), m_enabled(true), m_provider(NULL)
{
    qDebug() << "emblemPlugin constructor";
}

emblemPlugin::~emblemPlugin()
{
   qDebug() << "emblemPlugin destructor";
   // 确保在插件销毁时注销provider
   if (m_provider)
   {
       qDebug() << "注销BadgeEmblemProvider实例";
       Peony::EmblemProviderManager::getInstance()->unregisterProvider(m_provider);
       m_provider->deleteLater();
       m_provider = nullptr;
   }
}

Peony::EmblemProvider *emblemPlugin::create()
{
    // 创建并返回自定义的EmblemProvider实例
    qDebug() << "create BadgeEmblemProvider";

    // 创建新的provider实例
    m_provider = new BadgeEmblemProvider();

    // 注册到EmblemProviderManager
    qDebug() << "注册BadgeEmblemProvider到EmblemProviderManager";
    Peony::EmblemProviderManager::getInstance()->registerProvider(m_provider);

    // 输出已注册的provider数量，用于调试
    qDebug() << "provider注册完成，当前已注册的provider数量: " << Peony::EmblemProviderManager::getInstance()->metaObject()->className();

    return m_provider;
}

// 实现PluginInterface接口方法
Peony::PluginInterface::PluginType emblemPlugin::pluginType()
{
    return Peony::PluginInterface::EmblemPlugin;
}

const QString emblemPlugin::name()
{
    return "Emblem Badge Plugin";
}

const QString emblemPlugin::description()
{
    return "文件角标插件，根据文件类型显示相应的角标图标";
}

const QIcon emblemPlugin::icon()
{
    return QIcon(); // 返回空图标
}

void emblemPlugin::setEnable(bool enable)
{
    m_enabled = enable;
}

bool emblemPlugin::isEnable()
{
    return m_enabled;
}
