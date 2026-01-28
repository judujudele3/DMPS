#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "helpers/SaveFileDialogHelper.hpp"
#include <QFileDialog>
#include <QDebug>
#include <QSettings>
#include <QStyleFactory>
#include <QActionGroup>
#include <QDesktopServices>
#include <QUrl>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
    , currentTheme_(Theme::Light){
    ui->setupUi(this);  // Initialise l'interface définie dans MainWindow.ui
    // connexion des actions définies dans le .ui
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onQuit);
    connect(ui->actionApply_Modules, &QAction::triggered, this, &MainWindow::onApplyModule);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAbout);
    connect(ui->actionDocumentation, &QAction::triggered, this, &MainWindow::onDocumentation);
    connect(ui->actionClose, &QAction::triggered, this, &MainWindow::onClose);
    // Liste des actions non implémentées
    QList<QAction*> notImplemented = {
        ui->actionCopy,
        ui->actionCut,
        ui->actionFind,
        ui->actionPaste,
        ui->actionUndo,
        ui->actionManage_Modules,
        ui->actionLanguage,
        ui->actionPreferences,
        ui->actionToggle_Panels,
        ui->actionZoom_In,
        ui->actionZoom_Out
    };

    // Connexion en masse
    for (QAction* action : notImplemented) {
        connect(action, &QAction::triggered, this, &MainWindow::onFeatureNotImplemented);
    }
    currentTheme_ = loadThemePreference();
    applyTheme(currentTheme_);
    setupShortcuts();
    setupThemeMenu();
    setupDockWidgets();
    QSettings settings("YourCompany", "YourApp");
    restoreGeometry(settings.value("mainwindow/geometry").toByteArray());
    restoreState(settings.value("mainwindow/windowState").toByteArray());

    // Si première ouverture (pas de settings), maximiser
    if (settings.value("mainwindow/geometry").isNull()) {
        showMaximized();
    }

}

MainWindow::~MainWindow() {
    // Sauvegarder l'état des docks et la géométrie de la fenêtre
    QSettings settings("YourCompany", "YourApp");
    settings.setValue("mainwindow/geometry", saveGeometry());
    settings.setValue("mainwindow/windowState", saveState());
    delete ui;
}

void MainWindow::setController(Controller* controller)
{
    m_controller = controller;

    connect(this, &MainWindow::enableDisableModulesRequested,
            m_controller, &Controller::onEnableDisableModules);
}

// MainWindow.cpp
void MainWindow::on_actionOpen_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Open file"),
        "",
        tr("All files (*.*)")
        );


    if (filePath.isEmpty())
        return;


    // appeler le controller
    if (m_controller)
        m_controller->openFile(filePath.toStdString());

    if (m_controller)
        qDebug() << "[DEBUG] stackedWidget pointer:" << ui->stackedWidget;
        qDebug() << "[DEBUG] stackedWidget visible:" << ui->stackedWidget->isVisible();

        m_controller->displayCurrentData(ui->stackedWidget);
}



void MainWindow::onQuit()
{
    // Demander confirmation si des données sont chargées
    if (m_controller && m_controller->hasData()) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "Quit Application",
            "Don't forget to save your work.\n\nWould you like to quit?",
            QMessageBox::Cancel | QMessageBox::Yes,
            QMessageBox::Cancel
            );

        if (reply == QMessageBox::Cancel) {
            return;
        }
    }

    close();  // Ferme la fenêtre principale
}

void MainWindow::onApplyModule() {
    if (m_controller) {
        m_controller->onApplyModules(this); // Passer 'this' comme parent
    } else {
        QMessageBox::warning(this, "Error", "Controller not initialized");
    }
}

void MainWindow::onAbout() {
    QMessageBox::about(this, "À propos",
                       "DMPS — Data & Media Processing Studio\nVersion prototype");
}

void MainWindow::on_actionEnable_Disable_triggered()
{
    emit enableDisableModulesRequested();
}

void MainWindow::on_actionSave_Save_As_triggered()
{
    if (!m_controller) {
        QMessageBox::warning(this, "Error", "Controller not initialized");
        return;
    }

    m_controller->saveDataAs(this);
}

void MainWindow::setupDockWidgets()
{
    // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    // RÉSULTATS EXPLORER (Gauche)
    // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    resultsExplorer_ = new ResultsExplorerWidget(this);

    // Position et zones autorisées
    addDockWidget(Qt::LeftDockWidgetArea, resultsExplorer_);
    resultsExplorer_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    // Features : movable, closable, floatable, mais PAS resizable verticalement
    resultsExplorer_->setFeatures(
        QDockWidget::DockWidgetMovable |
        QDockWidget::DockWidgetFloatable |
        QDockWidget::DockWidgetClosable
        );

    // Taille par défaut (largeur fixe raisonnable)
    resultsExplorer_->setMinimumWidth(300);
    resultsExplorer_->setMaximumWidth(600);

    // Caché par défaut
    resultsExplorer_->hide();

    // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    // MESSAGES LOG (Bas)
    // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    messagesLog_ = new MessagesLogWidget(this);

    // Position et zones autorisées
    addDockWidget(Qt::BottomDockWidgetArea, messagesLog_);
    messagesLog_->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);

    // Features
    messagesLog_->setFeatures(
        QDockWidget::DockWidgetMovable |
        QDockWidget::DockWidgetFloatable |
        QDockWidget::DockWidgetClosable
        );

    // Taille par défaut (hauteur raisonnable)
    messagesLog_->setMinimumHeight(150);
    messagesLog_->setMaximumHeight(400);

    // Caché par défaut
    messagesLog_->hide();

    // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    // CONFIGURATION DES TAILLES RELATIVES
    // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

    // Définir les proportions quand les docks sont visibles
    // Par défaut : Results = 25% largeur, Messages = 20% hauteur

    // Cette ligne force Qt à respecter les tailles relatives
    setCorner(Qt::BottomLeftCorner, Qt::BottomDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::BottomDockWidgetArea);

    // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    // MENU VIEW (pour afficher/masquer)
    // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

    // Créer ou récupérer le menu View
    QMenu* viewMenu = nullptr;

    // Chercher si un menu "View" existe déjà
    for (QAction* action : menuBar()->actions()) {
        if (action->text() == "View" || action->text() == "&View") {
            viewMenu = action->menu();
            break;
        }
    }

    // Sinon, créer le menu
    if (!viewMenu) {
        viewMenu = menuBar()->addMenu("&View");
    }

    // Ajouter les actions de toggle
    viewMenu->addAction(resultsExplorer_->toggleViewAction());
    viewMenu->addAction(messagesLog_->toggleViewAction());

    // Optionnel : Ajouter un séparateur et des actions de reset
    viewMenu->addSeparator();
    QAction* resetLayoutAction = viewMenu->addAction("Reset Layout");
    connect(resetLayoutAction, &QAction::triggered, this, &MainWindow::resetDockLayout);
}

void MainWindow::displayModuleResults(const std::vector<ModuleExecutionResult>& results)
{
    // Show panels if hidden
    if (resultsExplorer_->isHidden()) {
        resultsExplorer_->show();
    }
    if (messagesLog_->isHidden()) {
        messagesLog_->show();
    }

    // Display results
    resultsExplorer_->displayResults(results);
    messagesLog_->logResults(results);
}

void MainWindow::resetDockLayout()
{
    // Fermer tous les docks
    resultsExplorer_->hide();
    messagesLog_->hide();

    // Remettre aux positions par défaut
    addDockWidget(Qt::LeftDockWidgetArea, resultsExplorer_);
    addDockWidget(Qt::BottomDockWidgetArea, messagesLog_);

    // Restaurer les tailles
    resultsExplorer_->setMinimumWidth(300);
    resultsExplorer_->setMaximumWidth(600);
    messagesLog_->setMinimumHeight(150);
    messagesLog_->setMaximumHeight(400);

    QMessageBox::information(this, "Layout Reset",
                             "Dock widget layout has been reset to default.");
}



void MainWindow::setupShortcuts()
{
    // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    // FICHIERS
    // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    if (ui->actionOpen) {
        ui->actionOpen->setShortcut(QKeySequence::Open); // Ctrl+O
    }

    if (ui->actionSave_Save_As) {
        ui->actionSave_Save_As->setShortcut(QKeySequence::Save); // Ctrl+S
    }

    if (ui->actionExit) {
        ui->actionExit->setShortcut(QKeySequence::Quit); // Ctrl+Q
    }

    // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    // MODULES
    // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    if (ui->actionEnable_Disable) {
        ui->actionEnable_Disable->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M)); // Ctrl+M
    }

    if (ui->actionApply_Modules) {
        ui->actionApply_Modules->setShortcut(QKeySequence(Qt::Key_F5)); // F5
        // Ou Ctrl+R : ui->actionApply_Modules->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
    }

    // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    // AFFICHAGE / VIEW
    // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

    // Toggle Results Explorer (Ctrl+1)
    if (resultsExplorer_) {
        QAction* toggleResults = new QAction(this);
        toggleResults->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_1));
        connect(toggleResults, &QAction::triggered, [this]() {
            resultsExplorer_->setVisible(!resultsExplorer_->isVisible());
        });
        addAction(toggleResults);
    }

    // Toggle Messages Log (Ctrl+2)
    if (messagesLog_) {
        QAction* toggleMessages = new QAction(this);
        toggleMessages->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_2));
        connect(toggleMessages, &QAction::triggered, [this]() {
            messagesLog_->setVisible(!messagesLog_->isVisible());
        });
        addAction(toggleMessages);
    }

    // Plein écran (F11)
    QAction* toggleFullScreen = new QAction(this);
    toggleFullScreen->setShortcut(QKeySequence(Qt::Key_F11));
    connect(toggleFullScreen, &QAction::triggered, [this]() {
        if (isFullScreen()) {
            showMaximized();
        } else {
            showFullScreen();
        }
    });
    addAction(toggleFullScreen);

    // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    // AIDE
    // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    if (ui->actionAbout) {
        ui->actionAbout->setShortcut(QKeySequence(Qt::Key_F1)); // F1
    }
}

void MainWindow::applyTheme(Theme theme)
{
    currentTheme_ = theme;

    switch (theme) {
    case Theme::Light:
        applyLightTheme();
        break;
    case Theme::Dark:
        applyDarkTheme();
        break;
    }

    saveThemePreference(theme);
}

void MainWindow::applyLightTheme()
{
    // Revenir au thème par défaut du système
    qApp->setStyle(QStyleFactory::create("Fusion"));
    qApp->setPalette(style()->standardPalette());

    // Optionnel : Personnaliser un peu
    QPalette lightPalette;
    lightPalette.setColor(QPalette::Window, QColor(240, 240, 240));
    lightPalette.setColor(QPalette::WindowText, Qt::black);
    lightPalette.setColor(QPalette::Base, Qt::white);
    lightPalette.setColor(QPalette::AlternateBase, QColor(245, 245, 245));
    lightPalette.setColor(QPalette::ToolTipBase, Qt::white);
    lightPalette.setColor(QPalette::ToolTipText, Qt::black);
    lightPalette.setColor(QPalette::Text, Qt::black);
    lightPalette.setColor(QPalette::Button, QColor(240, 240, 240));
    lightPalette.setColor(QPalette::ButtonText, Qt::black);
    lightPalette.setColor(QPalette::BrightText, Qt::red);
    lightPalette.setColor(QPalette::Link, QColor(0, 100, 200));
    lightPalette.setColor(QPalette::Highlight, QColor(76, 163, 224));
    lightPalette.setColor(QPalette::HighlightedText, Qt::white);

    qApp->setPalette(lightPalette);
}

void MainWindow::applyDarkTheme()
{
    qApp->setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;

    // Couleurs de base
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(35, 35, 35));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);

    // Liens et sélection
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    // États désactivés
    darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127, 127, 127));
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));

    qApp->setPalette(darkPalette);

    // Style CSS pour certains widgets spécifiques
    qApp->setStyleSheet(
        "QToolTip { color: #ffffff; background-color: #2a2a2a; border: 1px solid #555; }"
        "QMenu::item:selected { background-color: #2a82da; }"
        );
}

void MainWindow::saveThemePreference(Theme theme)
{
    QSettings settings("YourCompany", "DMPS");
    settings.setValue("theme", static_cast<int>(theme));
}

Theme MainWindow::loadThemePreference()
{
    QSettings settings("YourCompany", "DMPS");
    int themeInt = settings.value("theme", static_cast<int>(Theme::Light)).toInt();
    return static_cast<Theme>(themeInt);
}


void MainWindow::setupThemeMenu()
{
    // Vérifier que les actions existent
    if (!ui->actionLight || !ui->actionDark) {
        qWarning() << "Theme actions not found in UI";
        return;
    }

    // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    // CONFIGURATION DES ACTIONS
    // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

    // Rendre les actions checkable (comme des radio buttons)
    ui->actionLight->setCheckable(true);
    ui->actionDark->setCheckable(true);

    // Grouper pour que seule une soit cochée à la fois
    QActionGroup* themeGroup = new QActionGroup(this);
    themeGroup->addAction(ui->actionLight);
    themeGroup->addAction(ui->actionDark);

    // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    // CONNEXIONS
    // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

    connect(ui->actionLight, &QAction::triggered, this, [this]() {
        applyTheme(Theme::Light);
    });

    connect(ui->actionDark, &QAction::triggered, this, [this]() {
        applyTheme(Theme::Dark);
    });

    // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    // ÉTAT INITIAL (cocher le thème actuel)
    // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

    if (currentTheme_ == Theme::Light) {
        ui->actionLight->setChecked(true);
    } else {
        ui->actionDark->setChecked(true);
    }
}

void MainWindow::onFeatureNotImplemented()
{
    QAction* action = qobject_cast<QAction*>(sender());
    QString featureName = action ? action->text().remove('&') : "Cette fonctionnalité";

    QMessageBox::information(
        this,
        "Fonctionnalité non implémentée",
        featureName + "\n\nCette fonctionnalité n'est pas encore implémentée."
        );
}

void MainWindow::onDocumentation()
{
    QDesktopServices::openUrl(QUrl("https://github.com/judujudele3/DMPS"));
}

void MainWindow::onClose()
{
    qDebug() << "[onClose] called";

    if (m_controller && m_controller->hasData()) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "Close File",
            "Don't forget to save your work.\n\nWould you like to close the current file?",
            QMessageBox::Cancel | QMessageBox::Yes,
            QMessageBox::Cancel
            );
        if (reply == QMessageBox::Cancel)
            return;
    }

    // Fermer le fichier
    if (m_controller) {
        m_controller->closeFile();
        qDebug() << "[onClose] File closed";
    }

    if (!ui->stackedWidget) {
        qDebug() << "[onClose] ERROR: stackedWidget is null!";
        return;
    }

    // Supprimer tous les widgets existants
    while (QWidget* w = ui->stackedWidget->currentWidget()) {
        ui->stackedWidget->removeWidget(w);
        w->deleteLater();
    }

    // Ajouter un widget vide
    QWidget* emptyWidget = new QWidget();
    ui->stackedWidget->addWidget(emptyWidget);
    ui->stackedWidget->setCurrentWidget(emptyWidget);

    qDebug() << "[onClose] stackedWidget reset with empty widget";
}


