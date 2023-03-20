

df = read.csv("./mean1.csv")
summary(df)
is.numeric(df$x)
plot(df)
hist(df$x)
View(df)
qqnorm(df$x); qqline(df$x)


df2 = read.csv("./median1.csv")
hist(df2$x)
summary(df2)
median(df2$x)
mean(df2$x)
qqnorm(df2$x); qqline(df2$x)


co = read.csv("./coffee-talk.csv")
View(co)
co = co[co$I.have.read..understood.and.signed.the.informed.consent.form.and.I.consent.to.taking.part.in.this.study.=="I consent",]
co$How.many.cups.of.coffee.do.you.drink.per.week.=as.numeric(as.character(co$How.many.cups.of.coffee.do.you.drink.per.week.))
summary(co$How.many.cups.of.coffee.do.you.drink.per.week.)
boxplot(co$How.many.cups.of.coffee.do.you.drink.per.week.)
co = co[co$How.many.cups.of.coffee.do.you.drink.per.week.<100,]
boxplot(co$How.many.cups.of.coffee.do.you.drink.per.week.)

co$Please.state.your.gender=as.factor(co$Please.state.your.gender)
hist(co$How.many.cups.of.coffee.do.you.drink.per.week)

a = aov(co$How.many.cups.of.coffee.do.you.drink.per.week.~co$Please.state.your.gender, co)
summary(a)

shapiro.test(a$residuals)
qqnorm(a$residuals); qqline(a$residuals)
leveneTest(co$How.many.cups.of.coffee.do.you.drink.per.week. ~ co$Please.state.your.gender, co)
t.test(co$How.many.cups.of.coffee.do.you.drink.per.week. ~ co$Please.state.your.gender, co)



