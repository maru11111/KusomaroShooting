#include "stdafx.h"
#include "Bullet.h"
#include "Common.h"
#include "Objects.h"

int BaseBullet::numInstances = 0;

Array<String> BaseBullet::kusomaroTexts =
{
	U"俺がしぐれういだ！",
	U"たわわに実ったヒラメ",
	U"ういンカー「曲がるが？」",
	U"ういング「飛ぶが？」",
	U"簡単なこともわからないわ\n和菓子ってパンだっけ？",
	U"黄色いトマトはスイカ",
	U"イラストレーターのしぐれういです！\n今日は1万4千個のマシュマロに回答していきます！\n多分30分くらいで終わるかな？\n\n\n       こ             な             れ             う             い",
	U"きれいに咲いたしらたき",
	U"鋭利なみかん",
	U"トントントントン　カニのうどん",
};

Array<String> BaseBullet::beamTexts =
{
	U"R難しいかもしれないけれど\nR今、転職を考えております\nR美術系の仕事を探しているのですが\nRいい感じの\nRうなぎ",
	U"Rうい先生に質問です絵を描き始めるのに\nRいち万円で板タブを譲ってもらったのですが\nRびみょうに古いです\nRいまからでも新型を買った方がいいですか？\nRむだにならない値段も教えてほしいです",
	U"ういRうRいうい麦畑でね\n君とR日なたぼっこ　眠りましょ\n仲良Rい　良い　良いとかよしてよね\n夢はR夢でやっぱお断り",
	U"嫌になっちゃRうぐらい\n伝え切れなRいから引き続き\n凸凹コンRビネーションで\nso YES R/ NOならどーなの？\nNO 成R仏してくださーい！",
	U"Rうに食べたい\nこRいも食べたい\n甘エRビもいいな\nアルコRールと一緒に\n海鮮系のRムニエルで",
	U"Rうちの娘がうい先生のファンで\nRいつもかじりついて配信を見ていますｗ\n\nR美術の授業でもうい先生の絵を描いて\nR一番かわいくできたと持って帰って飾ってます\nR娘は本当はいません"
};

BaseBullet::BaseBullet(Objects& objects_, Vec2 pos_)
	: objects{objects_}
	, pos{ pos_ }
{
	numInstances++;
	damageAmount = 10;
	//Print << numInstances;
}

BaseBullet::~BaseBullet() {
	numInstances--;
	//Print << numInstances;
}

void BaseBullet::update() {
	//前のフレームで判定
	if (isOffScreen()) {
		isDestroyNextFrame = true;
	}
	move();
}

void BaseBullet::setIsHit(bool isHit_) {
	isHit = isHit_;
}

bool BaseBullet::isOffScreen() {
	if (pos.x < 0 - TextureAsset(U"Marshmallow").size().x*3 || Scene::Size().x + TextureAsset(U"Marshmallow").size().x*3 < pos.x || pos.y < TextureAsset(U"UIBack").size().y*6 - TextureAsset(U"Marshmallow").size().y*3 || Scene::Size().y + TextureAsset(U"Marshmallow").size().y*3 < pos.y)return true;
	else return false;
}

bool BaseBullet::isDestroy() {
	if (isDestroyNextFrame)return true;
	//貫通攻撃に上方修正
	//else  if (isHit) return true;
	else return false;
}

int BaseBullet::getDamageAmount() {
	return damageAmount;
}

RectF BaseBullet::collision() {
	return RectF(Arg::center(pos), 7*3, 7*3);
}

MaroType BaseBullet::getType() {
	return type;
}

Vec2 BaseBullet::getPos() {
	return pos;
}

bool BaseBullet::isNotEnemyHit(int id) {
	for (int i = 0; i < idOfEnemyHitOnce.size(); i++) {
		if (idOfEnemyHitOnce[i] == id) return false;
	}
	return true;
}

bool BaseBullet::getIsHit() {
	return isHit;
}

void BaseBullet::addId(int id) {
	idOfEnemyHitOnce << id;
}

void BaseBullet::backGroundDraw()const {}

void BaseBullet::draw() {
	TextureAsset(U"Marshmallow").scaled(3).drawAt(pos);
	//Debug
	//RectF(Arg::center(pos), 20, 20).draw(ColorF(1, 0, 0));
}




NormalMarshmallow::NormalMarshmallow(Objects& objects_, Vec2 pos_)
	: BaseBullet(objects_, pos_)
{
	vec = { 1,0 };
	speed = 600;
	type = MaroType::Normal;
}

void NormalMarshmallow::move() {
	pos += vec * speed * Scene::DeltaTime();
}




KusoMarshmallowUp::KusoMarshmallowUp(Objects& objects_, Vec2 pos_)
	: BaseBullet(objects_, pos_)
{
	vec = { 1,0 };
	speed = 600;
	type = MaroType::Up;
}

void KusoMarshmallowUp::move()
{
	vec.y -= 1.5  * Scene::DeltaTime();
	pos += vec * speed * Scene::DeltaTime();
}

void KusoMarshmallowUp::draw() {
	TextureAsset(U"KusomaroUp").scaled(3).drawAt(pos);
	//Debug
	//RectF(Arg::center(pos), 20, 20).draw(ColorF(1, 0, 0));
}



KusoMarshmallowDown::KusoMarshmallowDown(Objects& objects_, Vec2 pos_)
	: BaseBullet(objects_, pos_)
{
	vec = { 1,0 };
	speed = 600;
	type = MaroType::Down;
}

void KusoMarshmallowDown::move()
{
	vec.y += 1.5 * Scene::DeltaTime();
	pos += vec * speed * Scene::DeltaTime();
}

void KusoMarshmallowDown::draw() {
	TextureAsset(U"KusomaroDown").scaled(3).drawAt(pos);
	//Debug
	//RectF(Arg::center(pos), 20, 20).draw(ColorF(1, 0, 0));
}



KusoMarshmallowSine::KusoMarshmallowSine(Objects& objects_, Vec2 pos_)
	: BaseBullet(objects_, pos_)
{
	vec = { 1,0 };
	speed =500;
	type = MaroType::Sine;
}

void KusoMarshmallowSine::move()
{
	vec.y = 1.5 * Math::Sin(timer*10 - 90_deg);
	pos += vec * speed * Scene::DeltaTime();

	timer += Scene::DeltaTime();
}

void KusoMarshmallowSine::draw() {
	TextureAsset(U"KusomaroSine").scaled(3).drawAt(pos);
	//Debug
	//RectF(Arg::center(pos), 7, 7).draw(ColorF(1, 0, 0));
}



KusoMarshmallowBeam::KusoMarshmallowBeam(Objects& objects_, Vec2 pos_)
	: BaseBullet(objects_, pos_)
{
	vec = { 1,0 };
	speed = 600;
	type = MaroType::Beam;
}

void KusoMarshmallowBeam::move()
{
	pos += vec * speed * Scene::DeltaTime();

	timer += Scene::DeltaTime();

	if (isHit) {
		AudioManager::Instance()->play(U"BeamStart");
		AudioManager::Instance()->play(U"Beam");
		objects.marshmallows << std::make_unique<Beam>(objects, objects.player->getPos());
		isShotBeam = true;
		isHit = false;
		objects.player->setIsBeamAttacking();
	}
}


bool KusoMarshmallowBeam::isDestroy() {
	//画面外に出たら
	if (isOffScreen()) {

		return true;
	}
	//ビームを撃ったら消える
	else  if (isShotBeam) return true;
	else return false;
}

void KusoMarshmallowBeam::draw() {
	TextureAsset(U"KusomaroBeam").scaled(3).drawAt(pos);
	//Debug
	//RectF(Arg::center(pos), 7, 7).draw(ColorF(1, 0, 0));
}


Beam::Beam(Objects& objects_, Vec2 pos_)
	: BaseBullet(objects, pos_)
{
	damageAmount = 4;
	type = MaroType::Empty;
}

void Beam::update() {

	//Print << U"ColliderOn" << isColliderActive;

	beamTimer += Scene::DeltaTime();
	beamColTimer += Scene::DeltaTime();

	if (isEndBeam) {
		isColliderActive = false;
	}

	if (isColliderActive) {
		isColliderActive = false;
	}

	if (beamTimer <= beamColTime) {
		backGroundOpacity += 0.8*Scene::DeltaTime();
	}

	if (beamColTime <= beamColTimer) {
		isColliderActive = true;
		beamColTimer -= beamColTime;
	}
}

RectF Beam::collision() {
	if (isColliderActive) return RectF(pos.movedBy(60, -TextureAsset(U"UiBeam").size().y / 15.0 - 32), Min(TextureAsset(U"UiBeam").size().x / 5 * 3, Scene::Size().x), TextureAsset(U"UiBeam").size().y / 15 * 3);
	else RectF(0, 0, 0, 0);
}

void Beam::move() {
	//動かない
}

bool Beam::isDestroy() {
	if (isEndBeam) return true;
	else false;
}

void Beam::backGroundDraw()const {
	Scene::Rect()
		.draw(ColorF{ 0, 0, 0, Min( ((int)(10*backGroundOpacity))/10.0, 0.5)});
}

void Beam::draw() {
	//スプライトシートを再生
	if (not isEndBeam) {
		int n = (int)(beamTimer / 0.05) % (5 * 16);
		//Print << n;
		//最後のシートに到達したか
		if (n == 5 * 16 - 1) isEndBeam = true;

		TextureAsset(U"UiBeam")((n % 5) * TextureAsset(U"UiBeam").size().x / 5, (int)(n / 5) * TextureAsset(U"UiBeam").size().y / 16, TextureAsset(U"UiBeam").size().x / 5, TextureAsset(U"UiBeam").size().y / 16).scaled(3).draw(pos.movedBy(40, -TextureAsset(U"UiBeam").size().y / 15.0 - 32));
	}

	//if(isColliderActive)RectF(pos.movedBy(60, -TextureAsset(U"UiBeam").size().y / 15.0 - 32), Min(TextureAsset(U"UiBeam").size().x / 5 * 3, Scene::Size().x), TextureAsset(U"UiBeam").size().y / 15 * 3).draw();
}
