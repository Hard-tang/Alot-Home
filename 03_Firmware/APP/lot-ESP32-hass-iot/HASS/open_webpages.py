import webbrowser
import yaml
import os

def open_webpages_from_yaml(yaml_file):
    try:
        with open(yaml_file, 'r', encoding='utf-8') as file:
            urls = yaml.safe_load(file)
        
        if not isinstance(urls, list):
            print("YAML文件中的URL列表格式不正确。请确保URL列表以短横线开头。")
            return
        
        # 指定Edge浏览器路径
        edge_path = 'msedge'
        webbrowser.register('edge', None, webbrowser.BackgroundBrowser(edge_path))
        
        for url in urls:
            if isinstance(url, str):
                webbrowser.get('edge').open(url)
            else:
                print(f"无效的URL格式: {url}")
    except FileNotFoundError:
        print(f"文件 {yaml_file} 未找到。")
    except yaml.YAMLError as exc:
        print(f"YAML解析错误: {exc}")

if __name__ == "__main__":
    yaml_file = 'webpages.yaml'  # YAML文件路径
    open_webpages_from_yaml(yaml_file)