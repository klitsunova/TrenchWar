#include "main_controller.h"
#include <iostream>

MainController::MainController(QWidget* parent)
    : QWidget(parent),
      music_player_(new QMediaPlayer(this)) {
  menu_controller_ = new MenuController(this);

  ConnectUI();

  CreateAudioOutput();

  music_player_->setSource(QUrl("qrc:Resources/Music/gameplay_sound_1.mp3"));
  music_player_->play();
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
  menu_controller_->SetGameStarted();
  events_controller_ = new EventsController(this);
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
  connect(events_controller_,
          &EventsController::ReturnToMainMenu,
          this,
          &MainController::ReturnToMenu);
  connect(events_controller_,
          &EventsController::HideMainMenu,
          menu_controller_,
          &MenuController::HideMenu);
}

void MainController::Exit() {
  QApplication::exit(0);
}

void MainController::ChangeMusicVolume() {
  std::cerr << "change volume:" << Settings::GetMusicVolume() << "\n";
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
  audioOutput->setVolume(Settings::GetMusicVolume() /
      static_cast<double>(Settings::kMaxVolume - Settings::kMinVolume));
  music_player_->setAudioOutput(audioOutput);
  music_player_->setLoops(QMediaPlayer::Infinite);
  music_player_->play();
}
