#include "applicationui.hpp"
#include "filedataicon.hpp"
#include "filedatafactory.hpp"
#include "filedatalistmodel.hpp"
#include "imagedata.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include <bps/navigator_invoke.h>

using namespace bb::cascades;

///////////////////////////////////////////////////////////////////////////////

ApplicationUI::ApplicationUI(bb::cascades::Application *app)
: QObject(app)
, m_model(new FileDataListModel(this))
, m_dev_path("accounts/1000/shared")
, m_def_path(QDir::rootPath () + m_dev_path)
{
    // Register custom type to QML
    qmlRegisterType<ImageData>        ("com.leoz", 1, 0, "ImageData");
    qmlRegisterType<FileDataListModel>("com.leoz", 1, 0, "FileDataListModel");

    // prepare the localization
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);
    if(!QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()))) {
        // This is an abnormal situation! Something went wrong!
        // Add own code to recover here
        qWarning() << "Recovering from a failed connect()";
    }
    // initial load
    onSystemLanguageChanged();

    // Create the UI
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("_app", this);

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    // Set created root object as the application scene
    app->setScene(root);

    // Load file icons
    FileDataIcon::loadIcons();
}

ApplicationUI::~ApplicationUI()
{
    // Free file icons
    FileDataIcon::freeIcons();
}

///////////////////////////////////////////////////////////////////////////////

bool ApplicationUI::showFileList(const QString& path)
{
    qWarning() << "ApplicationUI::showFileList" << path;

    QFileInfo info(path);

    info = normalizeInfo(info);

    bool result(false);

    if (info.isDir()) {
    	if (info.isReadable() && info.isExecutable()) {
    	    result = true;
    	}
    }
    else {
    	if(!FileDataFactory::isSupportedImage(QUrl::fromLocalFile(info.filePath()))) {
        	invokeFile(info);
    	}
    }

    return result;
}

bool ApplicationUI::showImageView(const QString& path)
{
    QFileInfo info(path);

    info = normalizeInfo(info);

    return FileDataFactory::isSupportedImage(QUrl::fromLocalFile(info.filePath()));
}

QString ApplicationUI::getDefPath()
{
	return getPath(m_def_path);
}

QString ApplicationUI::getDefTitle()
{
	return getTitle(m_def_path);
}

QString ApplicationUI::getPath(const QString& path)
{
    QFileInfo info(path);

    if (info.fileName() == QString("..")) {
        info = info.canonicalFilePath();
    }

    return info.filePath();
}

QString ApplicationUI::getTitle(const QString& path)
{
    QFileInfo info(path);

    if (info.fileName() == QString("..")) {
        info = info.canonicalFilePath();
    }

    // Fix the root name
    if (info.isRoot()) {
    	return QString("/");
    }

    return info.fileName();
}

///////////////////////////////////////////////////////////////////////////////

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("everyfile_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}

///////////////////////////////////////////////////////////////////////////////

void ApplicationUI::invokeFile(const QFileInfo& info)
{
	navigator_invoke_invocation_t *invoke = NULL;
	navigator_invoke_invocation_create(&invoke);

	QUrl u(QUrl::fromLocalFile(info.filePath()));
	QString url(u.toString());
	QByteArray bytes = url.toUtf8();
	navigator_invoke_invocation_set_uri(invoke, bytes.constData());

	navigator_invoke_invocation_send(invoke);
	navigator_invoke_invocation_destroy(invoke);
}

QFileInfo ApplicationUI::normalizeInfo(QFileInfo& info)
{
    if (info.fileName() == QString("..")) {
        qWarning() << "App::normalizeInfo : shall be normalized!";
        info = info.canonicalFilePath();
        qWarning() << "App::normalizeInfo : normalized path: " << info.filePath();
    }

    return info;
}

///////////////////////////////////////////////////////////////////////////////
