#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <QFileInfo>

namespace bb
{
    namespace cascades
    {
        class Application;
        class LocaleHandler;
    }
}

class QTranslator;
class FileDataListModel;

class ApplicationUI : public QObject
{
    Q_OBJECT

public:

    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI();

    Q_INVOKABLE bool showFileList(const QString& path);
    Q_INVOKABLE bool showImageView(const QString& path);
    Q_INVOKABLE QString getDefPath();
    Q_INVOKABLE QString getDefTitle();
    Q_INVOKABLE QString getPath(const QString& path);
    Q_INVOKABLE QString getTitle(const QString& path);

private slots:
    void onSystemLanguageChanged();

private:
    void invokeFile(const QFileInfo& info);
    QFileInfo normalizeInfo(QFileInfo& info);

    // The model that contains the progress and image data
    FileDataListModel* m_model;

    // Default path
    QString m_dev_path;
    QString m_def_path;

    QTranslator* m_pTranslator;
    bb::cascades::LocaleHandler* m_pLocaleHandler;
};

#endif /* ApplicationUI_HPP_ */
