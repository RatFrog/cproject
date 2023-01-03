
cc.Class({
    extends: cc.Component,

    properties: {
        rope_node: {
            default: null,
            type : cc.Node,
        },
        cow_ins: {
            default: null,
            type: cc.Node,
        },
        rope_imgs: {
            default: [],
            type: cc.SpriteFrame,
        },
        cow_pretab: {
            default: null,
            type: cc.Prefab,
        },
        time: 0,
    },

    // LIFE-CYCLE CALLBACKS:

    onLoad() {
        this.success = false;
        this.scoreNum = 0;
    },

    start () {
        let countDownlabel = cc.find("Canvas/bg_sprite/count_down").getComponent(cc.Label);
        countDownlabel.string = this.time + "s";
        this.schedule(function () {
            this.time--;
            countDownlabel.string = this.time + "s";
            if (this.time == 0) {
                cc.log("游戏结束");
                // 获取弹框节点
                let resultNode = cc.find("Canvas/result");
                // 获取title和content
                let titleNode = resultNode.getChildByName("title");
                let contentNode = resultNode.getChildByName("content");
                // 展示分数
                titleNode.getComponent(cc.Label).string = "最终得分 " + this.scoreNum;
                // 结果
                let contentLabel = contentNode.getComponent(cc.Label);
                switch (true) {
                    case this.scoreNum <= 3:
                        contentLabel.string = "套牛青铜"; 
                        break;
                    case this.scoreNum < 6:
                        contentLabel.string = "套牛高手";
                        break;
                    case this.scoreNum >= 6:
                        contentLabel.string = "套牛王者";
                        break;

                }
                resultNode.active = true;
                cc.director.pause();
            }
        },1);
    },

    // update (dt) {},

    /**
     * 捕获按钮点击事件
     * @param {any} event
     * @param {any} customEventDate
     */
    clickCapture: function (event, customEventDate) {
        this.rope_node.active = true;
        this.rope_node.setSiblingIndex(100);
        this.rope_node.y = -480;
        const up = cc.moveTo(0.5, this.rope_node.x, 0);

        // 判定结果
        let result = cc.callFunc(function () {
            // 获取当前牛的x点
            let curentX = this.cow_ins.x;
            if (curentX > -100 && curentX < 100) {
                cc.log("success!");
                // 移除
                let bgNode = this.node.getChildByName("bg_sprite");
                bgNode.removeChild(this.cow_ins);
                // 更换绳子
                let ropeType = this.cow_ins.getComponent("cow").randomType + 1;
                cc.log("=========="+ropeType);
                this.rope_node.getComponent(cc.Sprite).spriteFrame = this.rope_imgs[ropeType];
                // 生成一个新的牛
                this.cow_ins = cc.instantiate(this.cow_pretab);
                this.cow_ins.y = 0;
                bgNode.addChild(this.cow_ins);
                this.success = true;
                this.scoreNum++;
            } else {
                cc.log("fail!");
            }
        }, this);
        // 往回拉
        let down = cc.moveTo(0.5, this.rope_node.x, -600);
        // 重置绳子 
        let finish = cc.callFunc(function () {
            this.rope_node.getComponent(cc.Sprite).spriteFrame = this.rope_imgs[0];
            // 判断是否捕捉成功
            if (this.success == true) {
                let scoreLabel = cc.find("Canvas/bg_sprite/score").getComponent(cc.Label);
                scoreLabel.string = "Score:" + this.scoreNum;
                this.success = false;
            }
        },this);
        // 定义一个序列动画
        let sequence = cc.sequence(up, result, down, finish);
        this.rope_node.runAction(sequence);
    },
    closeBtn() {
        cc.log("继续游戏");
        cc.director.resume();
        cc.director.loadScene("game");
    }
});
