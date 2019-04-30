#include "Main.h"

GtkWidget *window;
GtkWidget *fixed;
GtkWidget *drawing_area;
GtkWidget *label;

extern int array[NUM][NUM];
extern int value[NUM][NUM][3];
extern int node[NUM * NUM][3];
extern int checknode[NUM][NUM];
extern int black[NUM * NUM][3];
extern int white[NUM * NUM][3]; 
extern int isfirststep;
extern int numnode;
extern int numblack, numwhite;
int winflag=0;

char talk1[4][MAXSIZE] = {"瞎猫碰着死耗子，你赢了。", \
	"竟然输给你了，晚节不保！", \
		"你赢了，别急着产生胜利的幻觉，你这是无心插柳柳成荫。", \
		"我输了，必须承认，你比我厉害！智商至少250。"};
char talk2[4][MAXSIZE] = {"输了吧，活该！呵呵。", \
	"我赢了。没错，我就是那个万里挑一的绝世高手！", \
		"我是赢家。啦啦啦，别问我是谁，我会让你输得很心碎！", \
		"你输了。生亦何欢，死亦何哀，节哀顺变！"};
char talk3[20][MAXSIZE] = {"你是男人还是女人？坦白说，我更愿意跟美女下棋。", \
	"我猜你绝非等闲之辈，肯定是个世外高手。", \
		"你有多大？放心，我问的是年龄！", \
		"你觉得，我棋下的怎么样?", \
		"我这一步好像是臭棋？", \
		"看来，我要使出杀手锏了。", \
		"你棋走得真好。官人，我还要！", \
		"居然让你领先一步，我找谁说理去！", \
		"你有几个兄弟姐妹？放心，我只是想在你输棋前多交个朋友而已。", \
		"不要只顾埋头下棋，你说句话。你该不会是美女吧？", \
		"据说美女都是冷若冰霜的，你说会不会是她们爱说冷笑话的缘故？", \
		"丑话说前头，你要敢悔棋，就别怪我翻脸不认人。", \
		"你好像快输了哦！要不要我让棋？", \
		"说起那个金刚圈，去年我在陈家村认识一个铁匠……我又走神了。", \
		"对了，你不介意我从你那拷点毛片吧？放心，我不用剪切！", \
		"你棋好像下的还不错，当然，比我还是差了点！", \
		"呵呵。（据说高手都喜欢“呵呵”）", \
		"你既然喜欢大刀阔斧的下棋，维护世界和平的重任就交给你了！", \
		"我有没有告诉过你？我努力读书只是为了更好的泡妞。", \
		"你这步棋是什么意思，葫芦里到底卖的什么药？ "};

gint area_redraw(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	printf("Draw background ......\n");
	int i, j;
	GdkColor     color;
	GdkDrawable     *canvas;
	GdkGC           *gc;
	canvas = widget->window;
	gc = widget->style->fg_gc[GTK_WIDGET_STATE(widget)];

	/* Draw background */   
	color.red = 65535;
	color.green = 47545;
	color.blue = 2855;
	gdk_gc_set_rgb_fg_color (gc, &color);
	for(i=0;i<DRAWING_AREA_WIDTH;i++)
		gdk_draw_line (canvas, gc, 0, i, DRAWING_AREA_WIDTH, i);

	/* Draw lines */
	color.red = 25723;
	color.green = 26985;
	color.blue = 5140;
	gdk_gc_set_rgb_fg_color (gc, &color);
	for(i=0; i<=DRAWING_AREA_WIDTH; i+=SIZE)
		gdk_draw_line (canvas, gc, i, 0, i, DRAWING_AREA_HEIGHT);
	for(i=0; i<=DRAWING_AREA_HEIGHT; i+=SIZE)
		gdk_draw_line (canvas, gc, 0, i, DRAWING_AREA_WIDTH, i);

	/* Draw chessman */
	for(i=0; i<NUM; i++)
		for(j=0; j<NUM; j++)
		{
			if(array[i][j]==HUMAN)
			{
				color.red = 10000;
				color.green = 10000;
				color.blue = 10000;
				gdk_gc_set_rgb_fg_color (gc, &color);
				gdk_draw_arc (canvas, gc, TRUE, ROWTOPOS(j), ROWTOPOS(i), CMANSIZE, CMANSIZE, 0, 360*64);
				gdk_draw_arc (canvas, gc, FALSE, ROWTOPOS(j), ROWTOPOS(i), CMANSIZE, CMANSIZE, 0, 360*64);
			}

			if(array[i][j]==COMPUTER)
			{
				color.red = 65535;
				color.green = 65535;
				color.blue = 65535;
				gdk_gc_set_rgb_fg_color (gc, &color);
				gdk_draw_arc (canvas, gc, TRUE, ROWTOPOS(j), ROWTOPOS(i), CMANSIZE, CMANSIZE, 0, 360*64);
				gdk_draw_arc (canvas, gc, FALSE, ROWTOPOS(j), ROWTOPOS(i), CMANSIZE, CMANSIZE, 0, 360*64);
			}
		}

	color.red = 0;
	color.green = 0;
	color.blue = 0;
	gdk_gc_set_rgb_fg_color (gc, &color);

	return TRUE;
}

gint area_click(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	int x1, y1, x2, y2, ran_num, rtn;
	rtn = 10;
	char words[MAXSIZE+10];
	char msg[MAXSIZE];
	static int count=0;

	x1 = y1 = x2 = y2 = 0;
	GdkModifierType state;
	GtkWidget    *dialog;
	GdkColor     color;
	GdkDrawable     *canvas;
	GdkGC           *gc;
	canvas = widget->window;
	gc = widget->style->fg_gc[GTK_WIDGET_STATE(widget)];

	count++;
	gdk_window_get_pointer(widget->window, &x1, &y1, &state);
	if(winflag == 0){
		color.red = 10000;
		color.green = 10000;
		color.blue = 10000;
		gdk_gc_set_rgb_fg_color (gc, &color);
		gdk_draw_arc (canvas, gc, TRUE, CURTOPOS(x1), CURTOPOS(y1), CMANSIZE, CMANSIZE, 0, 360*64);
		gdk_draw_arc (canvas, gc, FALSE, CURTOPOS(x1), CURTOPOS(y1), CMANSIZE, CMANSIZE, 0, 360*64);
	}

	//10000为黑色 65535为白色
	if(winflag==0)
		rtn = play(CURTOROW(x1), CURTOROW(y1), &x2, &y2);

	printf("**********   %d %d\n", count, rtn);

	if(rtn<0)
	{
		color.red = 10000;
		color.green = 10000;
		color.blue = 10000;
		gdk_gc_set_rgb_fg_color (gc, &color);
		memset(words, 0, sizeof(words));
		snprintf(words,sizeof(words)-1, "[电脑]说：请重新开始吧！");
		gtk_label_set_text(label, words);

		/*显示对话框*/
		memset(msg, 0, sizeof(msg));
		snprintf(msg, sizeof(msg)-1, "可能已平局？！");
		dialog=gtk_message_dialog_new(GTK_WINDOW(window),0,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,msg);
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
	}

	if(winflag==0 &&rtn==0)
	{
		color.red = 65535;
		color.green = 65535;
		color.blue = 65535;
		gdk_gc_set_rgb_fg_color (gc, &color);
		gdk_draw_arc (canvas, gc, TRUE, ROWTOPOS(x2), ROWTOPOS(y2), CMANSIZE, CMANSIZE, 0, 360*64);
		gdk_draw_arc (canvas, gc, FALSE, ROWTOPOS(x2), ROWTOPOS(y2), CMANSIZE, CMANSIZE, 0, 360*64);

		color.red = 10000;
		color.green = 10000;
		color.blue = 10000;
		gdk_gc_set_rgb_fg_color (gc, &color);
		srand((unsigned)time(0)+count);
		ran_num = rand()%20;
		memset(words, 0, sizeof(words));
		snprintf(words,sizeof(words)-1, "[电脑]说：%s", talk3[ran_num]);
		//        snprintf(words,sizeof(words)-1, "x2[%d], y2[%d], rtn[%d], winflag[%d]", x2, y2, rtn, winflag );
		gtk_label_set_text(label, words);
	}

	if(winflag==0 &&rtn==HUMAN)
	{
		color.red = 65535;
		color.green = 65535;
		color.blue = 65535;
		gdk_gc_set_rgb_fg_color (gc, &color);
		gdk_draw_arc (canvas, gc, TRUE, ROWTOPOS(x2), ROWTOPOS(y2), CMANSIZE, CMANSIZE, 0, 360*64);
		gdk_draw_arc (canvas, gc, FALSE, ROWTOPOS(x2), ROWTOPOS(y2), CMANSIZE, CMANSIZE, 0, 360*64);

		color.red = 10000;
		color.green = 10000;
		color.blue = 10000;
		gdk_gc_set_rgb_fg_color (gc, &color);

		winflag = 1;
		srand((unsigned)time(0)+count);
		ran_num = rand()%4;
		memset(words, 0, sizeof(words));
		snprintf(words, sizeof(words)-1, "[电脑]说：%s", talk1[ran_num]);
		gtk_label_set_text(label, words);

		/*显示对话框*/
		memset(msg, 0, sizeof(msg));
		snprintf(msg, sizeof(msg)-1, "[玩家] 获胜\n共 [%d] 步棋", count);
		dialog=gtk_message_dialog_new(GTK_WINDOW(window),0,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,msg);
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
	}

	if(winflag==0 &&rtn==COMPUTER)
	{
		color.red = 65535;
		color.green = 65535;
		color.blue = 65535;
		gdk_gc_set_rgb_fg_color (gc, &color);
		gdk_draw_arc (canvas, gc, TRUE, ROWTOPOS(x2), ROWTOPOS(y2), CMANSIZE, CMANSIZE, 0, 360*64);
		gdk_draw_arc (canvas, gc, FALSE, ROWTOPOS(x2), ROWTOPOS(y2), CMANSIZE, CMANSIZE, 0, 360*64);

		winflag = 1;
		color.red = 10000;
		color.green = 10000;
		color.blue = 10000;
		gdk_gc_set_rgb_fg_color (gc, &color);
		srand((unsigned)time(0)+count);
		ran_num = rand()%4;
		memset(words, 0, sizeof(words));
		snprintf(words,sizeof(words)-1, "[电脑]说：%s", talk2[ran_num]);
		gtk_label_set_text(label, words);

		/*显示对话框*/
		memset(msg, 0, sizeof(msg));
		snprintf(msg, sizeof(msg)-1, "[电脑] 获胜\n共 [%d] 步棋", count);
		dialog=gtk_message_dialog_new(GTK_WINDOW(window),0,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,msg);
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
	}

	if(rtn==3)
	{
		color.red = 10000;
		color.green = 10000;
		color.blue = 10000;
		gdk_gc_set_rgb_fg_color (gc, &color);
		memset(words, 0, sizeof(words));
		snprintf(words,sizeof(words)-1, "[电脑]说：请不要耍赖哦！");
		gtk_label_set_text(label, words);

		/*显示对话框*/
		memset(msg, 0, sizeof(msg));
		snprintf(msg, sizeof(msg)-1, "不符合游戏规则！");
		dialog=gtk_message_dialog_new(GTK_WINDOW(window),0,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,msg);
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
	}
	return TRUE;
}

int main(int argc, char **argv)
{
	isfirststep = 1, numnode = -1;
	numwhite = -1, numblack = -1;
	memset(value, 0, sizeof(value));
	memset(node, 0, sizeof(node));
	memset(checknode, 0, sizeof(checknode));
	memset(black, 0, sizeof(black));
	memset(white, 0, sizeof(white));

	gtk_set_locale();
	gtk_init(&argc, &argv);

	/* Create a new window */
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_usize(window, WINWIDTH, WINHEIGHT);
	gtk_container_set_border_width(GTK_CONTAINER(window), WINDOW_BORDER_WIDTH);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_title (GTK_WINDOW(window), "五子棋");
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
	g_signal_connect_swapped(G_OBJECT(window),  "destroy", G_CALLBACK(gtk_main_quit), G_OBJECT(window));

	/* Create a new drawing area */
	drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request(drawing_area, DRAWING_AREA_WIDTH, DRAWING_AREA_HEIGHT);

	/* Add events */
	g_signal_connect(G_OBJECT(drawing_area), "expose_event", G_CALLBACK(area_redraw), NULL);
	gtk_widget_add_events(drawing_area, GDK_BUTTON_PRESS_MASK);
	g_signal_connect (GTK_OBJECT(drawing_area), "button_press_event", GTK_SIGNAL_FUNC(area_click), (gpointer) drawing_area);

	/* Create a new text */
	label = gtk_label_new(" ");
	gtk_widget_set_size_request(label, LABELWIDTH, LABELHEIGHT);
	gtk_label_set_line_wrap (label, TRUE);

	fixed = gtk_fixed_new();
	gtk_widget_set_usize(fixed,20,20);
	gtk_fixed_put(GTK_FIXED(fixed),label,0,0);
	gtk_fixed_put(GTK_FIXED(fixed),drawing_area,0,LABELHEIGHT+5);
	gtk_container_add(GTK_CONTAINER(window), fixed);

	/* Initialize array */
	int i, j;
	for(i=0; i<NUM;i++)
	{
		for(j=0; j<NUM;j++)
			array[i][j] = EMPTY;
	}

	/* show all windows */

	gtk_widget_show_all (window);
	gtk_main ();
	return 0;
}
