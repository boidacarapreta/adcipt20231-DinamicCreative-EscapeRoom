export default class aviso_hora2 extends Phaser.Scene {
  constructor() {
    super("aviso-hora");
  }

  preload() {
    this.load.image(
      "botao-desistencia",
      "./assets/desistir/botaodesistencia.png"
    );
    this.load.image(
      "caixa-desistencia",
      "./assets/desistir/caixadesistencia.png"
    );
    this.load.image("botao-nao", "./assets/desistir/botaonao.png");
    this.load.image("botao-sim", "./assets/desistir/botaosim.png");
    this.load.image("aviso-hora", "./assets/cenaavisohora.png");
  }

  create() {
    this.botao_desistencia = this.add
      .image(400, 50, "botao-desistencia")
      .setInteractive()
      .on("pointerdown", () => {
        thid.botao_desistencia.destroy();
        this.desistir = this.add.image(225, 400, "caixa-desistencia");

        /*
          .image(325, 400, "botao-nao")
          .image(125, 400, "botao-sim")

          .setInteractive()
          .on("pointerdown", () => {
            this.timedEvent.destroy();
            this.timerText.destroy();
            this.botao_alerta.destroy();
            this.desistir.destroy();
            this.game.scene.start("cena-desistencia");
          });
          */
      });

    /* Contagem regressiva */
    this.timer = 10;

    this.timedEvent = this.time.addEvent({
      delay: 1000,
      callback: this.countdown,
      callbackScope: this,
      loop: true,
    });

    // Relógio em tela
    this.timerText = this.add.text(225, 400, this.timer, {
      fontSize: "64px",
      fill: "#FFFFFF",
    });
  }

  upload() {}

  countdown() {
    /* Reduz o contador em 1 segundo */
    this.timer -= 1;

    /* Quando chegar a zero, troca a cena */
    if (this.timer === 0) {
      this.timedEvent.destroy();
      this.timerText.destroy();
      this.botao_alerta.destroy();
      this.game.scene.start("j1n1");
    } else {
      this.timerText.setText(this.timer);
    }
  }
}