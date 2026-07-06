pragma Singleton
import QtQuick 2.15

/* ============================================================
   在线教育平台 - 全局主题定义
   ============================================================ */
QtObject {
    // 主色调
    readonly property color primaryColor: "#4A90D9"
    readonly property color primaryLight: "#7AB3F0"
    readonly property color primaryDark: "#2B6CB0"

    // 辅助色
    readonly property color accentColor: "#F5A623"
    readonly property color successColor: "#27AE60"
    readonly property color dangerColor: "#E74C3C"
    readonly property color warningColor: "#F39C12"
    readonly property color infoColor: "#3498DB"

    // 背景色
    readonly property color backgroundColor: "#F5F7FA"
    readonly property color surfaceColor: "#FFFFFF"
    readonly property color cardColor: "#FFFFFF"

    // 文字色
    readonly property color textPrimary: "#2D3748"
    readonly property color textSecondary: "#718096"
    readonly property color textLight: "#A0AEC0"
    readonly property color textOnPrimary: "#FFFFFF"

    // 边框
    readonly property color borderColor: "#E2E8F0"
    readonly property color dividerColor: "#EDF2F7"

    // 尺寸
    readonly property int fontSizeXs: 10
    readonly property int fontSizeSm: 12
    readonly property int fontSizeMd: 14
    readonly property int fontSizeLg: 16
    readonly property int fontSizeXl: 18
    readonly property int fontSizeXxl: 24
    readonly property int fontSizeTitle: 28

    // 间距
    readonly property int spacingXs: 4
    readonly property int spacingSm: 8
    readonly property int spacingMd: 12
    readonly property int spacingLg: 16
    readonly property int spacingXl: 24
    readonly property int spacingXxl: 32

    // 圆角
    readonly property int radiusSm: 4
    readonly property int radiusMd: 8
    readonly property int radiusLg: 12
    readonly property int radiusXl: 16

    // 阴影
    readonly property int shadowLevel1: 2
    readonly property int shadowLevel2: 4
    readonly property int shadowLevel3: 8

    // 动画时长
    readonly property int durationFast: 150
    readonly property int durationNormal: 250
    readonly property int durationSlow: 400

    // 导航栏
    readonly property int navBarWidth: 64
    readonly property int navBarExpandedWidth: 240

    // 视频播放器
    readonly property int videoControlsHeight: 48
    readonly property int danmakuAreaHeight: 0  // 叠加在视频上

    // 弹幕
    readonly property int danmakuDefaultSize: 28
    readonly property int danmakuSpeed: 200     // 像素/秒
}
