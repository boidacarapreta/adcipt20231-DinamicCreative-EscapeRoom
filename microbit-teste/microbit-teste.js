input.onButtonPressed(Button.A, function () {
  if (nivel == 5) {
    nivel = 0
  } else {
    nivel += 1
  }
  basic.showNumber(nivel)
})
input.onButtonPressed(Button.B, function () {
  music.play(music.tonePlayable(349, music.beat(BeatFraction.Whole)), music.PlaybackMode.InBackground)
  serial.writeLine('nivel' + convertToText(nivel))
})
let nivel = 0
nivel = 0
basic.showNumber(nivel)
