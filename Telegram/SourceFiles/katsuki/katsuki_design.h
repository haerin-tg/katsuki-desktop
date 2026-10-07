// Katsuki UI — design tokens (v2: flat / rounded / MD3).
/*
Katsuki UI — design tokens v2 ("flat, round, honest").

This is the single source of truth for the visual language. Values are
mirrored from `design.md` (intent) + `raw.md` (exact numbers) — keep the
three in sync when the design changes.

Design rules encoded here:
  * Flat surfaces only. No glass, blur, glow or decorative gradients.
  * One accent color: Telegram blue `#3390EC`. Everything else is neutral.
  * Rounded geometry: Material 3 shape scale for the player, product
    radii (14/18/24/28/pill) elsewhere.
  * The player speaks real Material 3 (color roles + type scale).

The token core is pure `constexpr` C++ (no Qt) so it can be verified
outside the build; `QColor` bridges are compiled in only when Qt headers
are available (they are, in the tdesktop build).

Themes: only the Light scheme is designed & locked (design.md). The
`Active` alias is what components should use; when the dark scheme gets
designed it replaces the alias target and nothing else changes.
*/
#pragma once

#include <cstdint>

#if defined(__has_include)
#  if __has_include(<QColor>)
#    include <QColor>
#    define KATSUKI_DESIGN_HAS_QT_COLOR 1
#  endif
#endif

namespace Katsuki {
namespace Design {

// ============ color primitive ============

struct Rgba {
	std::uint8_t r = 0;
	std::uint8_t g = 0;
	std::uint8_t b = 0;
	std::uint8_t a = 255;

	[[nodiscard]] constexpr std::uint32_t rgb() const {
		return (std::uint32_t(r) << 16) | (std::uint32_t(g) << 8) | b;
	}
	[[nodiscard]] constexpr std::uint32_t rgba() const {
		return (std::uint32_t(a) << 24) | rgb();
	}
};

[[nodiscard]] constexpr Rgba Rgb(std::uint32_t rgb) {
	return {
		std::uint8_t((rgb >> 16) & 0xFF),
		std::uint8_t((rgb >> 8) & 0xFF),
		std::uint8_t(rgb & 0xFF),
		255,
	};
}

// ============ shape — Material 3 scale (player) ============

namespace Shape {
inline constexpr auto kXs = 8;   // M3 extra-small
inline constexpr auto kS = 12;   // M3 small
inline constexpr auto kM = 16;   // M3 medium
inline constexpr auto kL = 20;   // M3 large
inline constexpr auto kXl = 28;  // M3 extra-large
inline constexpr auto kFull = 999; // M3 full (pill)
} // namespace Shape

// ============ radius — product scale (design.md "everything round") ============

namespace Radius {
inline constexpr auto kS = 14;      // rows, inputs' inner tiles
inline constexpr auto kM = 18;      // info container, channel card, compact row
inline constexpr auto kL = 24;      // profile panel, dialogs
inline constexpr auto kXl = 28;     // player card, sheets
inline constexpr auto kPill = 999;  // buttons, chips, tags
} // namespace Radius

// ============ type scale (MD3 names; px at 1.0 scale) ============

namespace Type {
inline constexpr auto kHeadlineSmall = 24.; // player title (400)
inline constexpr auto kTitle = 22.;         // profile name (600)
inline constexpr auto kTitleMedium = 16.;   // song card title (600)
inline constexpr auto kBody = 14.;          // rows, buttons (500)
inline constexpr auto kCaption = 12.5;      // secondary meta
inline constexpr auto kMicro = 11.;         // mono labels, timecodes

inline constexpr auto kWeightRegular = 400;
inline constexpr auto kWeightMedium = 500;
inline constexpr auto kWeightSemibold = 600;

// Geist (Vercel) is the product type family; Geist Mono for meta/timecodes.
inline constexpr auto kFontFamily = "Geist";
inline constexpr auto kFontMono = "Geist Mono";

// letter-spacing, em (CSS values from raw.md §1)
inline constexpr auto kSpacingBase = -0.011;
inline constexpr auto kSpacingTitle = -0.02;
inline constexpr auto kSpacingMicro = 0.13; // uppercase mono labels
} // namespace Type

// ============ spacing — 4px base grid ============

namespace Space {
inline constexpr auto kXs = 4;
inline constexpr auto kS = 8;
inline constexpr auto kM = 12;
inline constexpr auto kL = 16;
inline constexpr auto kXl = 20;
inline constexpr auto kXxl = 24;
} // namespace Space

// ============ palette — Light (the locked v2 scheme) ============

namespace Light {

// --- flat / v2 tokens (raw.md §1) ---
inline constexpr auto kBg = Rgb(0xFFFFFF);        // surfaces / cards
inline constexpr auto kCanvas = Rgb(0xE9EBEF);    // window background
inline constexpr auto kSubtle = Rgb(0xF3F4F6);    // tonal buttons, chips, tiles
inline constexpr auto kBorder = Rgb(0xE6E8EB);    // 1px borders / dividers
inline constexpr auto kText = Rgb(0x111418);      // primary text
inline constexpr auto kText2 = Rgb(0x656D76);     // secondary text

inline constexpr auto kAccent = Rgb(0x3390EC);    // THE accent (Telegram blue)
inline constexpr auto kAccentSoft = Rgb(0xE7F1FD); // card backgrounds, selected pills
inline constexpr auto kAccentTint = Rgb(0xD3E5F9); // decorative patterns

// text-on-accent-soft (badges, selected tabs)
inline constexpr auto kOnAccentSoft = Rgb(0x2374CC);
// profile avatar initials (on kAccentSoft)
inline constexpr auto kInitials = Rgb(0x2A72C4);
// channel pfp placeholder
inline constexpr auto kPfpBg = Rgb(0xDCE0E6);
inline constexpr auto kPfpText = Rgb(0x5A636E);

// --- Material 3 color roles (player only, raw.md §1) ---
namespace M3 {
inline constexpr auto kPrimary = Rgb(0x2E7CD6);
inline constexpr auto kOnPrimary = Rgb(0xFFFFFF);
inline constexpr auto kPrimaryContainer = Rgb(0xD6E8FB);
inline constexpr auto kOnPrimaryContainer = Rgb(0x0B2A47);

inline constexpr auto kSecondaryContainer = Rgb(0xE3E7ED);
inline constexpr auto kOnSecondaryContainer = Rgb(0x1B1F26);

inline constexpr auto kSurface = Rgb(0xFCFCFD);
inline constexpr auto kSurfaceContainerLow = Rgb(0xF7F8FA);
inline constexpr auto kSurfaceContainer = Rgb(0xF1F3F7);
inline constexpr auto kSurfaceContainerHigh = Rgb(0xEAEDF2);
inline constexpr auto kSurfaceContainerHighest = Rgb(0xE3E7ED);

inline constexpr auto kOnSurface = Rgb(0x191C21);
inline constexpr auto kOnSurfaceVariant = Rgb(0x565D67);
inline constexpr auto kOutline = Rgb(0xBFC5CD);
inline constexpr auto kOutlineVariant = Rgb(0xDEE2E8);

// slider track (inactive) = secondary container; handle = primary.
} // namespace M3

// --- waveform scrubber (provisional; MD3 slider is the primary control,
//     raw.md §4). Revisit when the player lands. ---
inline constexpr auto kWavePlayed = M3::kPrimary;
inline constexpr auto kWaveUnplayed = M3::kSecondaryContainer;
inline constexpr auto kWaveTick = M3::kOnPrimaryContainer;

} // namespace Light

// ============ active theme ============
// Components must use `Active`. Dark scheme is NOT designed yet
// (open pick in raw.md §7) — when it is, point this alias at it.

namespace Active = Light;

// ============ Qt bridges (tdesktop build only) ============

#ifdef KATSUKI_DESIGN_HAS_QT_COLOR

[[nodiscard]] inline QColor ToQColor(Rgba c) {
	return QColor(c.r, c.g, c.b, c.a);
}

namespace Qt {
[[nodiscard]] inline QColor Accent() { return ToQColor(Active::kAccent); }
[[nodiscard]] inline QColor AccentSoft() { return ToQColor(Active::kAccentSoft); }
[[nodiscard]] inline QColor Text() { return ToQColor(Active::kText); }
[[nodiscard]] inline QColor TextSecondary() { return ToQColor(Active::kText2); }
[[nodiscard]] inline QColor Background() { return ToQColor(Active::kBg); }
[[nodiscard]] inline QColor Canvas() { return ToQColor(Active::kCanvas); }
[[nodiscard]] inline QColor Subtle() { return ToQColor(Active::kSubtle); }
[[nodiscard]] inline QColor Border() { return ToQColor(Active::kBorder); }
[[nodiscard]] inline QColor WavePlayed() { return ToQColor(Active::kWavePlayed); }
[[nodiscard]] inline QColor WaveUnplayed() { return ToQColor(Active::kWaveUnplayed); }
[[nodiscard]] inline QColor WaveTick() { return ToQColor(Active::kWaveTick); }

// MD3 player roles (mini bar / player card, design.md §5).
[[nodiscard]] inline QColor PlayerSurface() { return ToQColor(Active::M3::kSurfaceContainer); }
[[nodiscard]] inline QColor PlayerPrimary() { return ToQColor(Active::M3::kPrimary); }
[[nodiscard]] inline QColor PlayerOnPrimary() { return ToQColor(Active::M3::kOnPrimary); }
[[nodiscard]] inline QColor PlayerPrimaryContainer() { return ToQColor(Active::M3::kPrimaryContainer); }
[[nodiscard]] inline QColor PlayerSecondaryContainer() { return ToQColor(Active::M3::kSecondaryContainer); }
[[nodiscard]] inline QColor PlayerOnSecondaryContainer() { return ToQColor(Active::M3::kOnSecondaryContainer); }
[[nodiscard]] inline QColor PlayerOnSurface() { return ToQColor(Active::M3::kOnSurface); }
[[nodiscard]] inline QColor PlayerOnSurfaceVariant() { return ToQColor(Active::M3::kOnSurfaceVariant); }
} // namespace Qt

#endif // KATSUKI_DESIGN_HAS_QT_COLOR

} // namespace Design
} // namespace Katsuki
