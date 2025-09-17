#!/usr/bin/env python3

import cgi
import cgitb
import os
import json
import urllib.parse
import hashlib
import secrets
import time
from http.cookies import SimpleCookie
from cryptography.fernet import Fernet
import base64

# Enable CGI error reporting
cgitb.enable()

# Configuration
SESSIONS_DIR = "/tmp/cgi_sessions"
SESSION_TIMEOUT = 86400  # 24 hours
ENCRYPTION_KEY_FILE = "/tmp/cgi_encryption_key"

def ensure_sessions_dir():
    """Create sessions directory if it doesn't exist"""
    if not os.path.exists(SESSIONS_DIR):
        os.makedirs(SESSIONS_DIR, mode=0o755)

def get_or_create_encryption_key():
    """Get existing encryption key or create new one"""
    if os.path.exists(ENCRYPTION_KEY_FILE):
        with open(ENCRYPTION_KEY_FILE, 'rb') as f:
            return f.read()
    else:
        key = Fernet.generate_key()
        with open(ENCRYPTION_KEY_FILE, 'wb') as f:
            f.write(key)
        return key

def encrypt_data(data, key):
    """Encrypt data using Fernet encryption"""
    f = Fernet(key)
    json_data = json.dumps(data).encode()
    encrypted = f.encrypt(json_data)
    return base64.urlsafe_b64encode(encrypted).decode()

def decrypt_data(encrypted_data, key):
    """Decrypt data using Fernet encryption"""
    try:
        f = Fernet(key)
        encrypted_bytes = base64.urlsafe_b64decode(encrypted_data.encode())
        decrypted = f.decrypt(encrypted_bytes)
        return json.loads(decrypted.decode())
    except Exception:
        return []

def generate_session_id():
    """Generate a secure session ID"""
    return secrets.token_urlsafe(32)

def get_session_file_path(session_id):
    """Get the file path for a session"""
    # Use hash to avoid directory traversal attacks
    safe_id = hashlib.sha256(session_id.encode()).hexdigest()
    return os.path.join(SESSIONS_DIR, f"session_{safe_id}.json")

def cleanup_old_sessions():
    """Remove expired session files"""
    if not os.path.exists(SESSIONS_DIR):
        return
    
    current_time = time.time()
    for filename in os.listdir(SESSIONS_DIR):
        if filename.startswith("session_"):
            filepath = os.path.join(SESSIONS_DIR, filename)
            try:
                # Check file modification time
                if current_time - os.path.getmtime(filepath) > SESSION_TIMEOUT:
                    os.remove(filepath)
            except OSError:
                pass  # File might have been deleted by another process

def load_session_data(session_id, encryption_key):
    """Load session data from file"""
    session_file = get_session_file_path(session_id)
    if os.path.exists(session_file):
        try:
            with open(session_file, 'r') as f:
                session_data = json.load(f)
                # Decrypt todo list
                if 'encrypted_todos' in session_data:
                    session_data['todos'] = decrypt_data(session_data['encrypted_todos'], encryption_key)
                else:
                    session_data['todos'] = []
                return session_data
        except (json.JSONDecodeError, OSError):
            return None
    return None

def save_session_data(session_id, todos, encryption_key, is_new_session=False):
    """Save session data to file"""
    session_file = get_session_file_path(session_id)
    session_data = {
        'session_id': session_id,
        'created': time.time(),
        'last_updated': time.time(),
        'encrypted_todos': encrypt_data(todos, encryption_key),
        'is_new_session': is_new_session
    }
    
    try:
        with open(session_file, 'w') as f:
            json.dump(session_data, f)
        return True
    except OSError:
        return False

def get_cookies():
    """Parse cookies from environment"""
    cookie_string = os.environ.get('HTTP_COOKIE', '')
    cookies = SimpleCookie()
    if cookie_string:
        cookies.load(cookie_string)
    return cookies

def generate_html(todos, session_info):
    """Generate the HTML page"""
    
    # Generate todo items HTML
    todo_items_html = ""
    for i, item in enumerate(todos):
        completed_class = "completed" if item.get('completed', False) else ""
        checked = "checked" if item.get('completed', False) else ""
        todo_items_html += f"""
        <div class="todo-item {completed_class}" id="item-{i}">
            <input type="checkbox" {checked} onchange="toggleTodo({i})">
            <span class="todo-text">{item['text']}</span>
            <button class="delete-btn" onclick="deleteTodo({i})">Delete</button>
        </div>
        """
    
    # Session status message
    status_message = ""
    if session_info['is_new']:
        status_message = '<div class="cookie-message new-session">🆕 New session started! Your todos are encrypted and stored securely.</div>'
    else:
        status_message = f'<div class="cookie-message existing-session">🔐 Welcome back! Session active since {time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(session_info["created"]))}.</div>'
    
    html = f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Secure CGI To-Do List</title>
    <style>
        * {{
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }}
        
        body {{
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
            padding: 20px;
        }}
        
        .container {{
            background: white;
            border-radius: 15px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.2);
            padding: 30px;
            width: 100%;
            max-width: 500px;
            animation: fadeIn 0.5s ease-in;
        }}
        
        @keyframes fadeIn {{
            from {{ opacity: 0; transform: translateY(20px); }}
            to {{ opacity: 1; transform: translateY(0); }}
        }}
        
        h1 {{
            text-align: center;
            color: #333;
            margin-bottom: 30px;
            font-size: 2.2em;
            background: linear-gradient(45deg, #667eea, #764ba2);
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
            background-clip: text;
        }}
        
        .cookie-message {{
            padding: 12px;
            border-radius: 8px;
            margin-bottom: 20px;
            text-align: center;
            font-size: 14px;
            font-weight: 500;
        }}
        
        .cookie-message.new-session {{
            background: #d4edda;
            color: #155724;
            border: 1px solid #c3e6cb;
            animation: pulse 2s ease-in-out;
        }}
        
        .cookie-message.existing-session {{
            background: #cce7ff;
            color: #004085;
            border: 1px solid #b3d7ff;
        }}
        
        @keyframes pulse {{
            0%, 100% {{ transform: scale(1); }}
            50% {{ transform: scale(1.02); }}
        }}
        
        .add-todo {{
            display: flex;
            margin-bottom: 25px;
            gap: 10px;
        }}
        
        .add-todo input {{
            flex: 1;
            padding: 12px 15px;
            border: 2px solid #e0e0e0;
            border-radius: 8px;
            font-size: 16px;
            transition: border-color 0.3s ease;
        }}
        
        .add-todo input:focus {{
            outline: none;
            border-color: #667eea;
            box-shadow: 0 0 0 3px rgba(102, 126, 234, 0.1);
        }}
        
        .add-todo button {{
            padding: 12px 20px;
            background: linear-gradient(45deg, #667eea, #764ba2);
            color: white;
            border: none;
            border-radius: 8px;
            font-size: 16px;
            cursor: pointer;
            transition: transform 0.2s ease;
            white-space: nowrap;
        }}
        
        .add-todo button:hover {{
            transform: translateY(-2px);
            box-shadow: 0 5px 15px rgba(102, 126, 234, 0.3);
        }}
        
        .todo-list {{
            max-height: 400px;
            overflow-y: auto;
        }}
        
        .todo-item {{
            display: flex;
            align-items: center;
            padding: 15px;
            margin-bottom: 10px;
            background: #f8f9fa;
            border-radius: 8px;
            transition: all 0.3s ease;
            animation: slideIn 0.3s ease;
        }}
        
        @keyframes slideIn {{
            from {{ opacity: 0; transform: translateX(-20px); }}
            to {{ opacity: 1; transform: translateX(0); }}
        }}
        
        .todo-item:hover {{
            background: #e9ecef;
            transform: translateX(5px);
        }}
        
        .todo-item.completed {{
            background: #e8f5e8;
            opacity: 0.7;
        }}
        
        .todo-item.completed .todo-text {{
            text-decoration: line-through;
            color: #6c757d;
        }}
        
        .todo-item input[type="checkbox"] {{
            margin-right: 15px;
            transform: scale(1.2);
            cursor: pointer;
        }}
        
        .todo-text {{
            flex: 1;
            font-size: 16px;
            color: #333;
            word-break: break-word;
        }}
        
        .delete-btn {{
            background: #dc3545;
            color: white;
            border: none;
            padding: 8px 12px;
            border-radius: 5px;
            cursor: pointer;
            font-size: 12px;
            transition: all 0.2s ease;
        }}
        
        .delete-btn:hover {{
            background: #c82333;
            transform: scale(1.05);
        }}
        
        .stats {{
            text-align: center;
            margin-top: 25px;
            padding-top: 20px;
            border-top: 1px solid #e0e0e0;
            color: #666;
        }}
        
        .empty-state {{
            text-align: center;
            padding: 40px 20px;
            color: #666;
            font-style: italic;
        }}
        
        .cgi-info {{
            background: #f1f3f4;
            padding: 15px;
            border-radius: 8px;
            margin-top: 20px;
            font-size: 13px;
            color: #666;
            border-left: 4px solid #667eea;
        }}
        
        .security-info {{
            background: #fff3cd;
            border: 1px solid #ffeaa7;
            color: #856404;
            padding: 10px;
            border-radius: 5px;
            font-size: 12px;
            margin-top: 10px;
        }}
    </style>
</head>
<body>
    <div class="container">
        <h1>🔐 Secure CGI To-Do List</h1>
        
        {status_message}
        
        <form method="POST" class="add-todo">
            <input type="text" name="new_todo" placeholder="Add a new task..." maxlength="200" required>
            <button type="submit" name="action" value="add">Add Task</button>
        </form>
        
        <div class="todo-list">
            {todo_items_html if todos else '<div class="empty-state">No tasks yet. Add one above! 📝</div>'}
        </div>
        
        {f'<div class="stats">📊 Total: {len(todos)} | ✅ Completed: {sum(1 for item in todos if item.get("completed", False))} | 📋 Pending: {sum(1 for item in todos if not item.get("completed", False))}</div>' if todos else ''}
        
        <div class="cgi-info">
            <strong>🚀 CGI Session Info:</strong><br>
            Server Software: {os.environ.get('SERVER_SOFTWARE', 'Unknown')}<br>
            Request Method: {os.environ.get('REQUEST_METHOD', 'Unknown')}<br>
            Session ID: {session_info['session_id'][:16]}...<br>
            Cookie Status: {'🍪 Existing session' if not session_info['is_new'] else '🆕 New session (Set-Cookie sent)'}
            
            <div class="security-info">
                🔐 <strong>Security Features:</strong> Data encrypted with Fernet • Session files in /tmp • Auto-cleanup after 24h • Secure session IDs
            </div>
        </div>
    </div>

    <script>
        function toggleTodo(index) {{
            submitAction('toggle', index);
        }}
        
        function deleteTodo(index) {{
            if (confirm('Are you sure you want to delete this task?')) {{
                submitAction('delete', index);
            }}
        }}
        
        function submitAction(action, index) {{
            const form = document.createElement('form');
            form.method = 'POST';
            form.style.display = 'none';
            
            const actionInput = document.createElement('input');
            actionInput.name = 'action';
            actionInput.value = action;
            
            if (index !== undefined) {{
                const indexInput = document.createElement('input');
                indexInput.name = 'index';
                indexInput.value = index;
                form.appendChild(indexInput);
            }}
            
            form.appendChild(actionInput);
            document.body.appendChild(form);
            form.submit();
        }}
    </script>
</body>
</html>"""
    return html

def main():
    # Initialize
    ensure_sessions_dir()
    cleanup_old_sessions()
    encryption_key = get_or_create_encryption_key()
    
    # Parse cookies
    cookies = get_cookies()
    session_id = cookies.get('session_id').value if 'session_id' in cookies else None
    
    # Load or create session
    session_data = None
    is_new_session = False
    
    if session_id:
        session_data = load_session_data(session_id, encryption_key)
    
    if not session_data:
        # Create new session
        session_id = generate_session_id()
        session_data = {
            'session_id': session_id,
            'created': time.time(),
            'todos': [],
            'is_new_session': True
        }
        is_new_session = True
    
    todos = session_data.get('todos', [])
    
    # Handle POST requests
    if os.environ.get('REQUEST_METHOD') == 'POST':
        form = cgi.FieldStorage()
        action = form.getvalue('action', '')
        
        if action == 'add':
            new_todo = form.getvalue('new_todo', '').strip()
            if new_todo and len(new_todo) <= 200:
                todos.append({'text': new_todo, 'completed': False})
        
        elif action == 'toggle':
            try:
                index = int(form.getvalue('index', -1))
                if 0 <= index < len(todos):
                    todos[index]['completed'] = not todos[index].get('completed', False)
            except (ValueError, IndexError):
                pass
        
        elif action == 'delete':
            try:
                index = int(form.getvalue('index', -1))
                if 0 <= index < len(todos):
                    todos.pop(index)
            except (ValueError, IndexError):
                pass
    
    # Save session data
    save_session_data(session_id, todos, encryption_key, is_new_session)
    
    # Prepare session info for HTML
    session_info = {
        'session_id': session_id,
        'created': session_data.get('created', time.time()),
        'is_new': is_new_session
    }
    
    # Generate HTML content first
    html = generate_html(todos, session_info)
    
    # Calculate content length in bytes
    content_bytes = html.encode('utf-8')
    content_length = len(content_bytes)
    
    # Send headers with Content-Length
    print("Content-Type: text/html\r")
    print(f"Content-Length: {content_length}\r")
    
    # Only send Set-Cookie for new sessions
    if is_new_session:
        print(f"Set-Cookie: session_id={session_id}; Max-Age={SESSION_TIMEOUT}; Path=/; HttpOnly; SameSite=Strict\r")
    
    print("\r")  # Empty line to end headers
    
    # Send HTML content
    print(html, end='')  # Use end='' to avoid adding extra newline

if __name__ == "__main__":
    main()