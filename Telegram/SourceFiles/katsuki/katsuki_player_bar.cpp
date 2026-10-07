// Katsuki UI module.
/*
Katsuki UI — MD3 mini player bar.
Geometry mirrors player-v2.html "mini": padding 10×14, gap 14,
48² thumb r12, 3px top progress line (primary), tonal 44² play button,
48² standard icon buttons. Tokens only from katsuki_design.h.
*/
#include "katsuki/katsuki_player_bar.h"

#include "katsuki/katsuki_design.h"

#include <QPainter>
#include <QPainterPath>
#include <QFont>
#include <QFontMetrics>
#include <QMouseEvent>

#include <algorithm>

namespace Katsuki {
namespace {

// --- geometry (player-v2.html "mini") ---
constexpr auto kBarHeight = 68.; // thumb 48 + 2×10 padding
constexpr auto kPadding = 14.;
constexpr auto kGap = 14.;
constexpr auto kThumb = 48.;
constexpr auto kThumbRadius = 12.; // Shape::kS
constexpr auto kBarRadius = 20.;  // Shape::kL
constexpr auto kProgressHeight = 3.;
constexpr auto kPlayButton = 44.;
constexpr auto kIconButton = 48.;
constexpr auto kIconGlyph = 20.; // prev/next glyph viewport (24-grid)
constexpr auto kPlayGlyph = 18.; // play/pause glyph viewport (24-grid)
constexpr auto kScrubBand = 6.;  // top band = progress scrubbing

[[nodiscard]] QPointF MouseLocalPosition(QMouseEvent *e) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	return e->position();
#else // Qt 5
	return e->localPos();
#endif
}

// Weight on the 400..700 scale; Qt5 wants its 0..99 scale.
void SetWeight(QFont &font, int weight) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	font.setWeight(QFont::Weight(weight));
#else // Qt 5
	font.setWeight(weight * 100 / 8);
#endif
}

[[nodiscard]] QFont TitleFont() {
	auto font = QFont();
	font.setFamily(QString::fromUtf8(Design::Type::kFontFamily));
	font.setPixelSize(int(Design::Type::kBody)); // 14
	SetWeight(font, Design::Type::kWeightMedium);
	return font;
}

[[nodiscard]] QFont SubtitleFont() {
	auto font = QFont();
	font.setFamily(QString::fromUtf8(Design::Type::kFontFamily));
	font.setPixelSize(int(Design::Type::kCaption)); // 12.5 → 12
	SetWeight(font, Design::Type::kWeightRegular);
	return font;
}

struct Buttons {
	QRectF prev;
	QRectF play;
	QRectF next;
};

[[nodiscard]] Buttons ButtonsAt(int width, int height) {
	const auto right = width - kPadding;
	const auto centerY = height / 2.;
	auto result = Buttons();
	result.next = QRectF(
		right - kIconButton,
		centerY - kIconButton / 2.,
		kIconButton,
		kIconButton);
	result.play = QRectF(
		result.next.x() - kGap - kPlayButton,
		centerY - kPlayButton / 2.,
		kPlayButton,
		kPlayButton);
	result.prev = QRectF(
		result.play.x() - kGap - kIconButton,
		centerY - kIconButton / 2.,
		kIconButton,
		kIconButton);
	return result;
}

[[nodiscard]] QRectF GlyphBox(const QRectF &button, qreal size) {
	return QRectF(
		button.x() + (button.width() - size) / 2.,
		button.y() + (button.height() - size) / 2.,
		size,
		size);
}

// 24-grid glyph viewport → target box.
void PaintGlyphRect(
		QPainter &p,
		const QRectF &box,
		qreal x,
		qreal y,
		qreal w,
		qreal h) {
	const auto s = box.width() / 24.;
	p.drawRect(QRectF(box.x() + x * s, box.y() + y * s, w * s, h * s));
}

void PaintPlayGlyph(QPainter &p, const QRectF &box, const QColor &color) {
	const auto s = box.width() / 24.;
	p.setBrush(color);
	auto path = QPainterPath();
	path.moveTo(QPointF(box.x() + 8 * s, box.y() + 5.5 * s));
	path.lineTo(QPointF(box.x() + 8 * s, box.y() + 18.5 * s));
	path.lineTo(QPointF(box.x() + 19 * s, box.y() + 12 * s));
	path.closePath();
	p.drawPath(path);
}

void PaintPauseGlyph(QPainter &p, const QRectF &box, const QColor &color) {
	p.setBrush(color);
	PaintGlyphRect(p, box, 6, 6, 2.5, 12);
	PaintGlyphRect(p, box, 15.5, 6, 2.5, 12);
}

void PaintPrevGlyph(QPainter &p, const QRectF &box, const QColor &color) {
	const auto s = box.width() / 24.;
	p.setBrush(color);
	PaintGlyphRect(p, box, 6, 6, 2.5, 12);
	auto path = QPainterPath();
	path.moveTo(QPointF(box.x() + 19 * s, box.y() + 6 * s));
	path.lineTo(QPointF(box.x() + 19 * s, box.y() + 18 * s));
	path.lineTo(QPointF(box.x() + 9.5 * s, box.y() + 12 * s));
	path.closePath();
	p.drawPath(path);
}

void PaintNextGlyph(QPainter &p, const QRectF &box, const QColor &color) {
	const auto s = box.width() / 24.;
	p.setBrush(color);
	PaintGlyphRect(p, box, 15.5, 6, 2.5, 12);
	auto path = QPainterPath();
	path.moveTo(QPointF(box.x() + 5 * s, box.y() + 6 * s));
	path.lineTo(QPointF(box.x() + 5 * s, box.y() + 18 * s));
	path.lineTo(QPointF(box.x() + 14.5 * s, box.y() + 12 * s));
	path.closePath();
	p.drawPath(path);
}

} // namespace

PlayerBar::PlayerBar(QWidget *parent) : RpWidget(parent) {
	setAttribute(Qt::WA_OpaquePaintEvent, false);
}

void PlayerBar::setTitle(const QString &title) {
	_title = title;
	update();
}

void PlayerBar::setSubtitle(const QString &subtitle) {
	_subtitle = subtitle;
	update();
}

void PlayerBar::setProgress(float64 fraction) {
	_progress = std::clamp(fraction, 0., 1.);
	update();
}

void PlayerBar::setPlaying(bool playing) {
	_playing = playing;
	update();
}

bool PlayerBar::playing() const {
	return _playing;
}

void PlayerBar::seekFromMouse(QMouseEvent *e) {
	if (!seekRequested) {
		return;
	}
	const auto pos = MouseLocalPosition(e);
	const auto w = width();
	if (w > 0) {
		seekRequested(std::clamp(pos.x() / w, 0., 1.));
	}
}

void PlayerBar::mousePressEvent(QMouseEvent *e) {
	const auto pos = MouseLocalPosition(e);
	if (pos.y() <= kScrubBand) {
		_scrubbing = true;
		seekFromMouse(e);
		return;
	}
	const auto b = ButtonsAt(width(), height());
	if (b.prev.contains(pos)) {
		if (prevRequested) {
			prevRequested();
		}
	} else if (b.play.contains(pos)) {
		if (playPauseRequested) {
			playPauseRequested();
		}
	} else if (b.next.contains(pos)) {
		if (nextRequested) {
			nextRequested();
		}
	}
}

void PlayerBar::mouseMoveEvent(QMouseEvent *e) {
	if (_scrubbing) {
		seekFromMouse(e);
	}
}

void PlayerBar::mouseReleaseEvent(QMouseEvent *e) {
	_scrubbing = false;
}

void PlayerBar::paintEvent(QPaintEvent *e) {
	const auto w = width();
	const auto h = height();
	if (w <= 0 || h <= 0) {
		return;
	}
	auto p = QPainter(this);
	p.setRenderHint(QPainter::Antialiasing, true);

	const auto bar = QRectF(0., 0., w, h);

	// surface
	p.setPen(Qt::NoPen);
	p.setBrush(Design::Qt::PlayerSurface());
	p.drawRoundedRect(bar, kBarRadius, kBarRadius);

	// 3px top progress line (clipped to the rounded surface)
	const auto progressWidth = _progress * w;
	if (progressWidth > 0.) {
		auto clip = QPainterPath();
		clip.addRoundedRect(bar, kBarRadius, kBarRadius);
		p.setClipPath(clip);
		p.setBrush(Design::Qt::PlayerPrimary());
		p.drawRoundedRect(
			QRectF(0., 0., progressWidth, kProgressHeight),
			kProgressHeight / 2.,
			kProgressHeight / 2.);
		p.setClipping(false);
	}

	// thumb 48² r12
	p.setBrush(Design::Qt::PlayerPrimaryContainer());
	p.drawRoundedRect(
		QRectF(kPadding, (h - kThumb) / 2., kThumb, kThumb),
		kThumbRadius,
		kThumbRadius);

	// title + subtitle
	const auto buttons = ButtonsAt(w, h);
	const auto textLeft = kPadding + kThumb + kGap;
	const auto textWidth = int(buttons.prev.x() - kGap - textLeft);
	if (textWidth > 0) {
		p.setPen(Design::Qt::PlayerOnSurface());
		p.setFont(TitleFont());
		const auto title = QFontMetrics(TitleFont()).elidedText(
			_title,
			Qt::ElideRight,
			textWidth);
		p.drawText(
			QRectF(textLeft, 8., textWidth, 26.),
			int(Qt::AlignLeft | Qt::AlignVCenter),
			title);

		p.setPen(Design::Qt::PlayerOnSurfaceVariant());
		p.setFont(SubtitleFont());
		const auto subtitle = QFontMetrics(SubtitleFont()).elidedText(
			_subtitle,
			Qt::ElideRight,
			textWidth);
		p.drawText(
			QRectF(textLeft, 34., textWidth, 22.),
			int(Qt::AlignLeft | Qt::AlignVCenter),
			subtitle);
	}

	// buttons
	p.setPen(Qt::NoPen);
	PaintPrevGlyph(
		p,
		GlyphBox(buttons.prev, kIconGlyph),
		Design::Qt::PlayerOnSurface());
	PaintNextGlyph(
		p,
		GlyphBox(buttons.next, kIconGlyph),
		Design::Qt::PlayerOnSurface());
	p.setBrush(Design::Qt::PlayerSecondaryContainer());
	p.drawRoundedRect(buttons.play, kPlayButton / 2., kPlayButton / 2.);
	if (_playing) {
		PaintPauseGlyph(
			p,
			GlyphBox(buttons.play, kPlayGlyph),
			Design::Qt::PlayerOnSecondaryContainer());
	} else {
		PaintPlayGlyph(
			p,
			GlyphBox(buttons.play, kPlayGlyph),
			Design::Qt::PlayerOnSecondaryContainer());
	}
}

} // namespace Katsuki
