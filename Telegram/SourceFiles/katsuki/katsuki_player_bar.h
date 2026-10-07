// Katsuki UI module.
/*
Katsuki UI — MD3 mini player bar.
Part of the Katsuki UI redesign for this Telegram Desktop fork.

design.md §5 "Mini player", measurements from raw.md §2 / player-v2.html:
surface-container bar (r20) with a 3px top progress line, 48² thumb (r12),
title/subtitle, standard icon buttons (prev/next) and a tonal play button.

Pure UI — playback state is pushed in via setters, actions come out as
std::function callbacks (same convention as WaveformWidget; no moc needed).
*/
#pragma once

#include "ui/rp_widget.h"

#include <QString>
#include <functional>

namespace Katsuki {

class PlayerBar final : public Ui::RpWidget {
public:
	explicit PlayerBar(QWidget *parent = nullptr);

	void setTitle(const QString &title);
	void setSubtitle(const QString &subtitle); // "artist · 1:34 / 3:24"
	void setProgress(float64 fraction);       // 0..1
	void setPlaying(bool playing);
	[[nodiscard]] bool playing() const;

	// user actions
	std::function<void()> playPauseRequested;
	std::function<void()> prevRequested;
	std::function<void()> nextRequested;
	std::function<void(float64)> seekRequested; // 0..1

protected:
	void paintEvent(QPaintEvent *e) override;
	void mousePressEvent(QMouseEvent *e) override;
	void mouseMoveEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;

private:
	void seekFromMouse(QMouseEvent *e);

	QString _title;
	QString _subtitle;
	float64 _progress = 0.;
	bool _playing = false;
	bool _scrubbing = false;
};

} // namespace Katsuki
