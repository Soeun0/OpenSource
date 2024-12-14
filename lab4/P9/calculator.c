#include <gtk/gtk.h>
#include <stdlib.h>  // atoi, atof 등 함수를 사용하기 위한 헤더 파일

GtkWidget *entry;

// 간단한 사칙연산 계산기
static void on_button_clicked(GtkWidget *button, gpointer data) {
    const gchar *text = gtk_button_get_label(GTK_BUTTON(button));
    const gchar *current_text = gtk_entry_get_text(GTK_ENTRY(entry));
    gchar *new_text = g_strconcat(current_text, text, NULL);
    gtk_entry_set_text(GTK_ENTRY(entry), new_text);
    g_free(new_text);
}

static void on_clear_clicked(GtkWidget *button, gpointer data) {
    gtk_entry_set_text(GTK_ENTRY(entry), "");
}

static void on_calculate_clicked(GtkWidget *button, gpointer data) {
    const gchar *expression = gtk_entry_get_text(GTK_ENTRY(entry));
    double result = 0;
    double num1, num2;
    char operator;

    // 입력된 식에서 첫 번째 숫자, 연산자, 두 번째 숫자를 추출합니다
    if (sscanf(expression, "%lf %c %lf", &num1, &operator, &num2) == 3) {
        switch (operator) {
            case '+':
                result = num1 + num2;
                break;
            case '-':
                result = num1 - num2;
                break;
            case '*':
                result = num1 * num2;
                break;
            case '/':
                if (num2 != 0) {
                    result = num1 / num2;
                } else {
                    gtk_entry_set_text(GTK_ENTRY(entry), "Error");
                    return;
                }
                break;
            default:
                gtk_entry_set_text(GTK_ENTRY(entry), "Invalid operator");
                return;
        }

        gchar result_str[50];
        snprintf(result_str, sizeof(result_str), "%f", result);
        gtk_entry_set_text(GTK_ENTRY(entry), result_str);
    } else {
        gtk_entry_set_text(GTK_ENTRY(entry), "Invalid input");
    }
}

int main(int argc, char *argv[]) {
    GtkWidget *window, *grid, *button, *clear_button, *calc_button;

    gtk_init(&argc, &argv);

    // 창 생성
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "간단한 계산기");
    gtk_window_set_default_size(GTK_WINDOW(window), 250, 350);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // 그리드 생성
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // 입력창 생성
    entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 4, 1);

    // 버튼들 생성
    const gchar *buttons[] = {
        "7", "8", "9", "/",
        "4", "5", "6", "*",
        "1", "2", "3", "-",
        "0", ".", "=", "+"
    };

    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            button = gtk_button_new_with_label(buttons[i * 4 + j]);
            g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);
            gtk_grid_attach(GTK_GRID(grid), button, j, i + 1, 1, 1);
        }
    }

    // 'C' (Clear) 버튼
    clear_button = gtk_button_new_with_label("C");
    g_signal_connect(clear_button, "clicked", G_CALLBACK(on_clear_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), clear_button, 0, 5, 2, 1);

    // '=' (Calculate) 버튼
    calc_button = gtk_button_new_with_label("=");
    g_signal_connect(calc_button, "clicked", G_CALLBACK(on_calculate_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), calc_button, 2, 5, 2, 1);

    // 창 보이기
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
