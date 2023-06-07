import React,{Component} from 'react'
import CssBaseline from '@material-ui/core/CssBaseline';
import Container from '@material-ui/core/Container';
import Board from './Board'
import Comment from './Comment'
import List from '@material-ui/core/List';
import axios from 'axios';


class App extends Component<any,any>{
    constructor(props:any){
        super(props) 
        this.state={
            list:[]
        }
      
    }
    componentDidMount(){
        this.query();
    }
    render(){
        return(
            <React.Fragment>
                <CssBaseline />
                    <Container maxWidth="sm">
                        <Comment 
                        addList={this.addList.bind(this)}/>
                        <List>
                        {
                            this.state.list.map((item:any)=>{
                                return (
                                     <Board 
                                        key = {item.author}
                                        author ={item.author} 
                                        comment = {item.content}
                                    />
                                )
                            })
                        }
                        </List>
                    </Container>
          </React.Fragment>
        )
    }
    query = () =>{
        axios.get('http://localhost:8080/api/getComments').then(({data})=>{
            this.setState({
                list:data
            })
        });
    }

    

    addList(newAuthor:any,newComment:any){
        console.log(newAuthor)
        console.log(newComment)
        axios.post("http://localhost:8080/api/addComment",{
            author : newAuthor,
            content : newComment
        }).then(({data}) => {
            console.log(newAuthor)
            this.query()
           // alert(data)
        })

    }
}

export default App;