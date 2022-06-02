#include "main_controller.h"

MainController::MainController(QWidget* parent)
    : QWidget(parent),
      music_player_(new QMediaPlayer(this)) {
  Settings::SetStandardSettings();
  menu_controller_ = new MenuController(this);

  ConnectUI();

  CreateAudioOutput();
}

void MainController::ConnectUI() {
  connect(menu_controller_,
          &MenuController::StartGame,
          this,
          &MainController::StartGame);
  connect(menu_controller_,
          &MenuController::ResumeGame,
          this,
          &MainController::ResumeGame);
  connect(menu_controller_,
          &MenuController::BackToMenu,
          this,
          &MainController::ReturnToMenu);
  connect(menu_controller_,
          &MenuController::Exit,
          this,
          &MainController::Exit);
  connect(menu_controller_,
          &MenuController::MusicVolumeChanged,
          this,
          &MainController::ChangeMusicVolume);
  connect(menu_controller_,
          &MenuController::FullScreenValueChanged,
          this,
          &MainController::ChangeScreenValue);
}

void MainController::StartGame() {
  menu_controller_->HideMenu();
  menu_controller_->SetGameStarted();
  events_controller_ = new EventsController(this);
  events_controller_->SetFullScreen(Settings::IsFullScreen());
  ConnectEventsControllerUI();
}

void MainController::ReturnToMenu() {
  events_controller_->HideGame();
  delete events_controller_;
  events_controller_ = nullptr;
  menu_controller_->HidePauseMenu();
  menu_controller_->SetGameFinished();
  menu_controller_->ShowMenu();
}

void MainController::PauseGame() {
  events_controller_->PauseTimer();
  menu_controller_->ShowPauseMenu();
}

void MainController::ResumeGame() {
  if (events_controller_->GetGameStage() == EventsController::Stage::kActive) {
    events_controller_->StartTimer();
  }
  menu_controller_->HidePauseMenu();
}

void MainController::ConnectEventsControllerUI() {
  connect(events_controller_,
          &EventsController::ShowPauseMenu,
          this,
          &MainController::PauseGame);
}

void MainController::Exit() {
  QApplication::exit(0);
}

void MainController::ChangeMusicVolume() {
  music_player_->audioOutput()->setVolume(Settings::GetMusicVolume() /
      static_cast<double>(Settings::kMaxVolume - Settings::kMinVolume));
}

void MainController::ChangeScreenValue() {
  if (events_controller_ != nullptr) {
    events_controller_->SetFullScreen(Settings::IsFullScreen());
  }
}

void MainController::CreateAudioOutput() {
  auto* audioOutput = new QAudioOutput(this);
  // music_player_->setSource(QUrl("qrc:Resources/Music/gameplay_sound_1.mp3"));
  audioOutput->setVolume(Settings::GetMusicVolume() /
      static_cast<double>(Settings::kMaxVolume - Settings::kMinVolume));
  music_player_->setAudioOutput(audioOutput);
  music_player_->setLoops(QMediaPlayer::Infinite);
  music_player_->play();
}
